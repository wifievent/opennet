#include "ethpacket.h"

void EthPacket::parse()
{
	ethHdr_ = PEthHdr(buf_.data_);
	switch (ethHdr_->type())
  {
		case EthHdr::Ip4:
			Buf backup = buf_;
			buf_.data_ += sizeof(EthHdr);
			buf_.size_ -= sizeof(EthHdr);
			IpPacket::parse();
			buf_ = backup;
			break;
		case EthHdr::Arp:
			arpHdr_ = PArpHdr(buf_.data_ + sizeof(EthHdr));
			break;
	}
}
