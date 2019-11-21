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

#include "ChannelSubPub.h"
#include "../XmsgChannelStatusCfg.h"

ChannelSubPub::ChannelSubPub()
{

}

void ChannelSubPub::pub(shared_ptr<XmsgImHlrChannelStatusChangedNotice> notice)
{
	auto it = this->cgt2ap2ccid.find(notice->cgt());
	if (it == this->cgt2ap2ccid.end()) 
	{
		LOG_TRACE("no subscriber for this channel global title, notice: %s", notice->ShortDebugString().c_str())
		return;
	}
	auto ap = XmsgNeMgr::instance()->get(X_MSG_AP);
	if (ap == nullptr)
	{
		LOG_FAULT("can not found x-msg-ap group, notice: %s", notice->ShortDebugString().c_str())
		return;
	}
	for (auto iter = it->second->begin(); iter != it->second->end(); ++iter)
		this->pub2ap(ap, iter->second, notice);
}

void ChannelSubPub::pub(shared_ptr<XmsgImGroupChannelStatusChangedNotice> notice)
{
	LOG_FAULT("it`s a bug, incomplete")
}

void ChannelSubPub::sub(const string& ap, const string& ccid, shared_ptr<XmsgChannelStatusSubReq> req)
{
	shared_ptr<unordered_map<string, shared_ptr<unordered_set<string>>>> ccid2cgt;
	auto it = this->ap2ccid2cgt.find(ap);
	if (it == this->ap2ccid2cgt.end())
		ccid2cgt.reset(new unordered_map<string, shared_ptr<unordered_set<string>>>());
	else
		ccid2cgt = it->second;
	this->sub4ap(ccid2cgt, ap, ccid, req);
}

void ChannelSubPub::unSub(const string& ap, const string& ccid, shared_ptr<XmsgChannelStatusUnSubReq> req)
{
	auto it = this->ap2ccid2cgt.find(ap);
	if (it == this->ap2ccid2cgt.end() || it->second->empty())
		return;
	for (int i = 0; i < req->cgt_size(); ++i)
		this->unSub4ap(it->second, ap, ccid, req->cgt(i));
}

void ChannelSubPub::unSub(const string& ap, const string& ccid, const string& cgt)
{
	auto it = this->ap2ccid2cgt.find(ap);
	if (it == this->ap2ccid2cgt.end() || it->second->empty())
		return;
	this->unSub4ap(it->second, ap, ccid, cgt);
}

void ChannelSubPub::unSub(const string& ap, const string& ccid)
{
	auto it = this->ap2ccid2cgt.find(ap);
	if (it == this->ap2ccid2cgt.end() || it->second->empty())
		return;
	this->unSub4ap(it->second, ap, ccid);
}

void ChannelSubPub::sub4ap(shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>> ccid2cgt, const string& ap, const string& ccid, shared_ptr<XmsgChannelStatusSubReq> req)
{
	auto it = ccid2cgt->find(ccid);
	if (it == ccid2cgt->end())
	{
		shared_ptr<unordered_set<string>> set(new unordered_set<string>());
		for (int i = 0; i < req->cgt_size(); ++i)
		{
			set->insert(req->cgt(i));
			this->addCgt2ap2ccid(req->cgt(i), ap, ccid);
		}
		(*ccid2cgt)[ccid] = set;
		return;
	}
	for (int i = 0; i < req->cgt_size(); ++i)
	{
		it->second->insert(req->cgt(i));
		this->addCgt2ap2ccid(req->cgt(i), ap, ccid);
	}
}

void ChannelSubPub::unSub4ap(shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>> ccid2cgt, const string& ap, const string& ccid, shared_ptr<XmsgChannelStatusUnSubReq> req)
{
	auto it = ccid2cgt->find(ccid);
	if (it == ccid2cgt->end())
		return;
	for (int i = 0; i < req->cgt_size(); ++i)
	{
		it->second->erase(req->cgt(i));
		this->delCgt2ap2ccid(req->cgt(i), ap, ccid);
	}
}

void ChannelSubPub::unSub4ap(shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>> ccid2cgt, const string& ap, const string& ccid, const string& cgt)
{
	auto it = ccid2cgt->find(ccid);
	if (it == ccid2cgt->end())
		return;
	it->second->erase(cgt);
	this->delCgt2ap2ccid(cgt, ap, ccid);
}

void ChannelSubPub::unSub4ap(shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>> ccid2cgt, const string& ap, const string& ccid)
{
	auto it = ccid2cgt->find(ccid);
	if (it == ccid2cgt->end())
		return;
	for (auto& iter : *(it->second))
		this->delCgt2ap2ccid(iter, ap, ccid);
	it->second->clear();
}

void ChannelSubPub::addCgt2ap2ccid(const string& cgt, const string& ap, const string& ccid)
{
	shared_ptr<unordered_map<string, shared_ptr<unordered_set<string>>>> ap2ccid;
	auto it = this->cgt2ap2ccid.find(cgt);
	if (it == this->cgt2ap2ccid.end()) 
	{
		ap2ccid.reset(new unordered_map<string, shared_ptr<unordered_set<string>>>());
		shared_ptr<unordered_set<string>> set(new unordered_set<string>());
		set->insert(ccid);
		(*ap2ccid)[ap] = set;
		this->cgt2ap2ccid[cgt] = ap2ccid;
		return;
	}
	ap2ccid = it->second;
	auto iter = ap2ccid->find(ap);
	if (iter == ap2ccid->end()) 
	{
		shared_ptr<unordered_set<string>> set(new unordered_set<string>());
		set->insert(ccid);
		(*ap2ccid)[ap] = set;
		return;
	}
	iter->second->insert(ccid);
}

void ChannelSubPub::delCgt2ap2ccid(const string& cgt, const string& ap, const string& ccid)
{
	auto it = this->cgt2ap2ccid.find(cgt);
	if (it == this->cgt2ap2ccid.end())
		return;
	shared_ptr<unordered_map<string, shared_ptr<unordered_set<string>>>> ap2ccid = it->second;
	auto iter = ap2ccid->find(ap);
	if (iter == ap2ccid->end())
		return;
	ap2ccid->erase(ccid);
}

void ChannelSubPub::pub2ap(shared_ptr<XmsgNeUsr> ap, shared_ptr<unordered_set<string>> ccid, shared_ptr<XmsgImHlrChannelStatusChangedNotice> notice)
{
	if (ccid->empty()) 
		return;
	shared_ptr<XmsgChannelStatusPubUsrStatusNotice> n(new XmsgChannelStatusPubUsrStatusNotice());
	n->set_cgt(notice->cgt());
	n->set_plat(notice->plat());
	n->set_did(notice->cgt());
	n->set_status(notice->status());
	for (auto& it : *ccid)
	{
		SptrOob oob(new list<pair<uchar, string>>());
		oob->push_back(make_pair<>(XSC_TAG_UID, it));
		XmsgImChannel::cast(ap->channel)->unidirection(n, oob);
	}
}

ChannelSubPub::~ChannelSubPub()
{

}

