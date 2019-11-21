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
#include "XmsgImGroup.h"

unordered_map<string, shared_ptr<XmsgImMsgStub>> XmsgImGroup::__h2nMsgs__;

XmsgImGroup::XmsgImGroup(shared_ptr<XscTcpServer> tcpServer, const string &peer, const string& pwd, const string& alg) :
		XmsgNe(tcpServer, peer, X_MSG_IM_GROUP, pwd, alg)
{
	for (auto& it : XmsgImGroup::__h2nMsgs__)
		this->msgMgr->msgs[it.first] = it.second;
}

void XmsgImGroup::authOk(shared_ptr<XmsgNeUsr> nu, shared_ptr<XmsgNeAuthRsp> rsp)
{
	this->subChannelStatus();
}

void XmsgImGroup::subChannelStatus()
{
	shared_ptr<XmsgImGroupChannelStatusSubReq> req(new XmsgImGroupChannelStatusSubReq());
	XmsgMisc::insertKv(req->mutable_ext(), "subscribe", "all");
	auto ne = static_pointer_cast<XmsgImGroup>(this->shared_from_this());
	this->begin(req, [ne](SptrXiti itrans)
	{
		if (itrans->ret != RET_SUCCESS)
		{
			LOG_ERROR("subscribe channel status from x-msg-im-group failed, channel: %s", ne->toString().c_str())
			ne->close(); 
			return;
		}
		LOG_INFO("subscribe channel status from x-msg-im-group successful, channel: %s", ne->toString().c_str())
	});
}

XscMsgItcpRetType XmsgImGroup::itcp(XscWorker* wk, XscChannel* channel, shared_ptr<XscProtoPdu> pdu)
{
	return XscMsgItcpRetType::DISABLE;
}

bool XmsgImGroup::regH2N(const Descriptor* begin, const Descriptor* end, const Descriptor* uni, void* cb, bool auth, ForeignAccessPermission foreign)
{
	shared_ptr<XmsgImMsgStub> stub(new XmsgImMsgStub(begin, end, uni, cb, auth, foreign));
	if (XmsgImGroup::__h2nMsgs__.find(stub->msg) != XmsgImGroup::__h2nMsgs__.end())
	{
		LOG_ERROR("duplicate message: %s", stub->msg.c_str())
		return false;
	}
	LOG_TRACE("reg h2n messsage: %s", stub->toString().c_str())
	XmsgImGroup::__h2nMsgs__[stub->msg] = stub;
	return true;
}

XmsgImGroup::~XmsgImGroup()
{

}

