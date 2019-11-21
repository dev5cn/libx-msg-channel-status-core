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

#ifndef CHANNEL_CHANNELSUBPUB_H_
#define CHANNEL_CHANNELSUBPUB_H_

#include <libx-msg-common-dat-struct-cpp.h>
#include <libx-msg-channel-status-pb.h>

class ChannelSubPub
{
public:
	void pub(shared_ptr<XmsgImHlrChannelStatusChangedNotice> notice); 
	void pub(shared_ptr<XmsgImGroupChannelStatusChangedNotice> notice); 
public:
	void sub(const string& ap, const string& ccid, shared_ptr<XmsgChannelStatusSubReq> req); 
	void unSub(const string& ap, const string& ccid, shared_ptr<XmsgChannelStatusUnSubReq> req); 
	void unSub(const string& ap, const string& ccid, const string& cgt); 
	void unSub(const string& ap, const string& ccid); 
	ChannelSubPub();
	virtual ~ChannelSubPub();
private:
	unordered_map<string , shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>>> ap2ccid2cgt; 
	unordered_map<string , shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>>> cgt2ap2ccid; 
private:
	void sub4ap(shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>> ccid2cgt, const string& ap, const string& ccid, shared_ptr<XmsgChannelStatusSubReq> req); 
	void unSub4ap(shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>> ccid2cgt, const string& ap, const string& ccid, shared_ptr<XmsgChannelStatusUnSubReq> req); 
	void unSub4ap(shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>> ccid2cgt, const string& ap, const string& ccid, const string& cgt); 
	void unSub4ap(shared_ptr<unordered_map<string , shared_ptr<unordered_set<string >>>> ccid2cgt, const string& ap, const string& ccid); 
private:
	void addCgt2ap2ccid(const string& cgt, const string& ap, const string& ccid); 
	void delCgt2ap2ccid(const string& cgt, const string& ap, const string& ccid); 
private:
	void pub2ap(shared_ptr<XmsgNeUsr> ap, shared_ptr<unordered_set<string>> ccid, shared_ptr<XmsgImHlrChannelStatusChangedNotice> notice); 
};

#endif 
