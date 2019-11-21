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

#ifndef XMSGCHANNELSTATUSCFG_H_
#define XMSGCHANNELSTATUSCFG_H_

#include <libx-msg-channel-status-pb.h>
#include <libx-msg-common-dat-struct-cpp.h>

class XmsgChannelStatusCfg
{
public:
	shared_ptr<XmsgChannelStatusCfgPb> cfgPb; 
	SptrCgt cgt; 
	SptrCgt uhlr; 
	SptrCgt ghlr; 
public:
	static shared_ptr<XmsgChannelStatusCfg> instance(); 
	static shared_ptr<XmsgChannelStatusCfg> load(const char* path); 
	static void setCfg(shared_ptr<XmsgChannelStatusCfg> cfg); 
	shared_ptr<XscTcpCfg> xscServerCfg(); 
	string toString();
	XmsgChannelStatusCfg();
	virtual ~XmsgChannelStatusCfg();
private:
	static shared_ptr<XmsgChannelStatusCfg> cfg; 
	bool loadLogCfg(XMLElement* root); 
	bool loadXscServerCfg(XMLElement* root); 
	bool loadXmsgNeH2nCfg(XMLElement* root); 
	bool loadXmsgNeN2hCfg(XMLElement* root); 
	bool loadMysqlCfg(XMLElement* root); 
	bool loadMongodbCfg(XMLElement* root); 
	bool loadKafkaProdCfg(XMLElement* root); 
	bool loadMiscCfg(XMLElement* root); 
};

#endif 
