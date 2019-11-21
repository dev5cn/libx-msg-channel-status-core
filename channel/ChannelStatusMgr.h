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

#ifndef CHANNEL_CHANNELSTATUSMGR_H_
#define CHANNEL_CHANNELSTATUSMGR_H_

#include "ChannelSubPub.h"

class ChannelStatusMgr: public ActorBlockingSingleThread
{
public:
	void query(const string& cgt, map<string , string >& usrClients); 
public:
	void upsert(shared_ptr<XmsgImHlrChannelStatusPubReq> pub); 
	void upsert(shared_ptr<XmsgImHlrChannelStatusChangedNotice> notice); 
	void upsert(shared_ptr<XmsgImGroupChannelStatusPubReq> pub); 
	void upsert(shared_ptr<XmsgImGroupChannelStatusChangedNotice> notice); 
	ChannelSubPub* getSubPub();
	static void init();
	static ChannelStatusMgr* instance();
private:
	void upsert4hlrNotice(const XmsgImHlrChannelStatusChangedNotice* notice); 
private:
	unordered_map<string , shared_ptr<unordered_map<string , string >>> channelStatus4usr; 
	unordered_map<string , string > channelStatus4ne; 
	unordered_map<string , string > channelStatus4group; 
	ChannelSubPub* subPub;
	static ChannelStatusMgr* inst;
	ChannelStatusMgr();
	virtual ~ChannelStatusMgr();
};

#endif 
