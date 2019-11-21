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
#include "XmsgImHlr.h"

unordered_map<string, shared_ptr<XmsgImMsgStub>> XmsgImHlr::__h2nMsgs__;

XmsgImHlr::XmsgImHlr(shared_ptr<XscTcpServer> tcpServer, const string &peer, const string& pwd, const string& alg) :
		XmsgNe(tcpServer, peer, X_MSG_IM_HLR, pwd, alg)
{
	for (auto& it : XmsgImHlr::__h2nMsgs__)
		this->msgMgr->msgs[it.first] = it.second;
}

void XmsgImHlr::authOk(shared_ptr<XmsgNeUsr> nu, shared_ptr<XmsgNeAuthRsp> rsp)
{
	this->subChannelStatus();
}

void XmsgImHlr::subChannelStatus()
{
	shared_ptr<XmsgImHlrChannelStatusSubReq> req(new XmsgImHlrChannelStatusSubReq());
	XmsgMisc::insertKv(req->mutable_ext(), "subscribe", "all");
	auto ne = static_pointer_cast<XmsgImHlr>(this->shared_from_this());
	this->begin(req, [ne](SptrXiti itrans)
	{
		if (itrans->ret != RET_SUCCESS)
		{
			LOG_ERROR("subscribe channel status from x-msg-im-hlr failed, channel: %s", ne->toString().c_str())
			ne->close(); 
			return;
		}
		LOG_INFO("subscribe channel status from x-msg-im-hlr successful, channel: %s", ne->toString().c_str())
	});
}

XscMsgItcpRetType XmsgImHlr::itcp(XscWorker* wk, XscChannel* channel, shared_ptr<XscProtoPdu> pdu)
{
	return XscMsgItcpRetType::DISABLE;
}

bool XmsgImHlr::regH2N(const Descriptor* begin, const Descriptor* end, const Descriptor* uni, void* cb, bool auth, ForeignAccessPermission foreign)
{
	shared_ptr<XmsgImMsgStub> stub(new XmsgImMsgStub(begin, end, uni, cb, auth, foreign));
	if (XmsgImHlr::__h2nMsgs__.find(stub->msg) != XmsgImHlr::__h2nMsgs__.end())
	{
		LOG_ERROR("duplicate message: %s", stub->msg.c_str())
		return false;
	}
	LOG_TRACE("reg h2n messsage: %s", stub->toString().c_str())
	XmsgImHlr::__h2nMsgs__[stub->msg] = stub;
	return true;
}

XmsgImHlr::~XmsgImHlr()
{

}

