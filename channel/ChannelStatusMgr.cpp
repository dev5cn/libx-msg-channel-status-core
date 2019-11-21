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

#include "ChannelStatusMgr.h"

ChannelStatusMgr* ChannelStatusMgr::inst;

ChannelStatusMgr::ChannelStatusMgr() :
		ActorBlockingSingleThread("channel-status")
{
	this->subPub = new ChannelSubPub();
}

void ChannelStatusMgr::init()
{
	ChannelStatusMgr::inst = new ChannelStatusMgr();
}

ChannelStatusMgr* ChannelStatusMgr::instance()
{
	return ChannelStatusMgr::inst;
}

ChannelSubPub* ChannelStatusMgr::getSubPub()
{
	return this->subPub;
}

void ChannelStatusMgr::query(const string& cgt, map<string, string>& usrClients)
{
	auto it = this->channelStatus4usr.find(cgt);
	if (it == this->channelStatus4usr.end())
		return;
	for (auto& iter : *(it->second))
	{
		if (iter.second == "offline")
			continue;
		usrClients[iter.first] = iter.second;
	}
}

void ChannelStatusMgr::upsert(shared_ptr<XmsgImHlrChannelStatusPubReq> pub)
{
	for (int i = 0; i < pub->notice_size(); ++i)
		this->upsert4hlrNotice(&pub->notice(i));
}

void ChannelStatusMgr::upsert(shared_ptr<XmsgImHlrChannelStatusChangedNotice> notice)
{
	this->upsert4hlrNotice(notice.get());
}

void ChannelStatusMgr::upsert(shared_ptr<XmsgImGroupChannelStatusPubReq> pub)
{
	LOG_FAULT("it`s a bug, incomplete")
}

void ChannelStatusMgr::upsert(shared_ptr<XmsgImGroupChannelStatusChangedNotice> notice)
{
	LOG_FAULT("it`s a bug, incomplete")
}

void ChannelStatusMgr::upsert4hlrNotice(const XmsgImHlrChannelStatusChangedNotice* notice)
{
	string platDid = XmsgMisc::makePlatDid(notice->plat(), notice->did());
	auto it = this->channelStatus4usr.find(notice->cgt());
	if (it == this->channelStatus4usr.end()) 
	{
		shared_ptr<unordered_map<string , string >> clients(new unordered_map<string, string>());
		clients->insert(make_pair<>(platDid, notice->status()));
		this->channelStatus4usr[notice->cgt()] = clients;
		return;
	}
	auto iter = it->second->find(platDid);
	if (iter == it->second->end())
	{
		it->second->insert(make_pair<>(platDid, notice->status()));
		return;
	}
	iter->second = notice->status();
}

ChannelStatusMgr::~ChannelStatusMgr()
{

}

