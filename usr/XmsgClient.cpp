/*
  Copyright 2019 www.dev5.cn, Inc. dev5@qq.com
 
  This file is part of X-MSG-IM.
 
  X-MSG-IM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  X-MSG-IM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with X-MSG-IM.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <libx-msg-channel-status-pb.h>
#include "XmsgClient.h"

XmsgClient::XmsgClient(const string& cgt, const string& plat, const string& did, const string& ccid)
{
	this->cgt = cgt;
	this->plat = plat;
	this->did = did;
	this->ccid = ccid;
}

string XmsgClient::toString()
{
	string str;
	SPRINTF_STRING(&str, "cgt: %s, plat: %s, did: %s, ccid: %s", this->cgt.c_str(), this->plat.c_str(), this->did.c_str(), this->ccid.c_str())
	return str;
}

XmsgClient::~XmsgClient()
{

}

