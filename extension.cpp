/**
 * vim: set ts=4 :
 * =============================================================================
 * Netprop Lookup
 * Copyright (C) 2015 Ross Bemrose (Powerlord).  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "eiface.h"

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */
NetpropLookup g_NetpropLookup;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_NetpropLookup);

bool NetpropLookup::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	return true;
}


// Taken from CHalfLife2

DataTableInfo *NetpropLookup::_FindServerClass(const char *classname)
{
	DataTableInfo *pInfo = NULL;
	if (!m_Classes.retrieve(classname, &pInfo))
	{
		ServerClass *sc = gamedll->GetAllServerClasses();
		while (sc)
		{
			if (strcmp(classname, sc->GetName()) == 0)
			{
				pInfo = new DataTableInfo(sc);
				m_Classes.insert(classname, pInfo);
				break;
			}
			sc = sc->m_pNext;
		}
		if (!pInfo)
			return NULL;
	}

	return pInfo;
}

// Taken from smn_entities

inline edict_t *BaseEntityToEdict(CBaseEntity *pEntity)
{
	IServerUnknown *pUnk = (IServerUnknown *)pEntity;
	IServerNetworkable *pNet = pUnk->GetNetworkable();

	if (!pNet)
	{
		return NULL;
	}

	return pNet->GetEdict();
}

// Modified from smn_entities

/* Given an entity reference or index, fill out a CBaseEntity and/or edict.
If lookup is successful, returns true and writes back the two parameters.
If lookup fails, returns false and doesn't touch the params.  */
bool IndexToAThings(cell_t num, CBaseEntity **pEntData, edict_t **pEdictData)
{
	CBaseEntity *pEntity = gamehelpers->ReferenceToEntity(num);

	if (!pEntity)
	{
		return false;
	}

	int index = gamehelpers->ReferenceToIndex(num);
	if (index > 0 && index <= playerhelpers->GetMaxClients())
	{
		IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(index);
		if (!pPlayer || !pPlayer->IsConnected())
		{
			return false;
		}
	}

	if (pEntData)
	{
		*pEntData = pEntity;
	}

	if (pEdictData)
	{
		edict_t *pEdict = ::BaseEntityToEdict(pEntity);
		if (!pEdict || pEdict->IsFree())
		{
			pEdict = NULL;
		}

		*pEdictData = pEdict;
	}

	return true;
}

static cell_t GetEntPathProp(IPluginContext *pContext, const cell_t *params)
{
	CBaseEntity *pEntity;
	char *prop;
	int offset;
	edict_t *pEdict;
	int bit_count;
	bool is_unsigned = false;

	prop_info_t propInfo;
	propInfo.index = params[1];

	if (!IndexToAThings(params[1], &propInfo.pEntity, &propInfo.pEdict))
	{
		return pContext->ThrowNativeError("Entity %d (%d) is invalid", gamehelpers->ReferenceToIndex(params[1]), params[1]);
	}

	pContext->LocalToString(params[3], &propInfo.prop);

}

bool FindPropSend(SendPropType type, const char *type_name, prop_info_t propInfo, char *error, int maxlen)
{
	sm_sendprop_info_t info;
	SendProp *pProp;
	IServerUnknown *pUnk = (IServerUnknown *)propInfo.pEntity;
	IServerNetworkable *pNet = pUnk->GetNetworkable();

	if (!pNet)
	{
		snprintf(error, maxlen, "Edict %d (%d) is not networkable", gamehelpers->ReferenceToIndex(propInfo.index), propInfo.index);
		return false;
	}

	if (!FindSendPropInfo(pNet->GetServerClass(), propInfo.prop, &info))
	{

	}
}

/*
The void pointer of base is important simply because it plus the offset are the ultimate location of what
we're looking for.

This is necessary for CUtlVector props, which relocate the base
*/
bool FindSendPropInTable(SendTable *pTable,
	const char *name,
	sm_sendprop_info_t *info,
	unsigned int offset,
	void *base)
{

}
