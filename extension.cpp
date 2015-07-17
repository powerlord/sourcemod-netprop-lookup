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
IServerGameDLL *gamedll = NULL;

NetpropLookup g_NetpropLookup;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_NetpropLookup);

bool NetpropLookup::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	GET_V_IFACE_ANY(GetServerFactory, gamedll, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);

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

