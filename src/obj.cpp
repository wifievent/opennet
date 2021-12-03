#include "obj.h"

Obj::~Obj() {
  if (state_ != Closed) {
    // critical error
    std::cout << "State is " << state_ << std::endl;
  }
  if (tempBuf_.data_ != nullptr) {
      delete[] tempBuf_.data_ ;
  }
  tempBuf_.clear();
}

bool Obj::active() {
    return state_ == Opened;
}

bool Obj::open() {
    if (state_ != Closed) {
    // not closed
    std::cout << "State is " << state_ << std::endl;
		return false;
	}

    state_ = Opening;
	bool res = doOpen();
	if (!res) {
        state_ = Closing;
		doClose();
        state_ = Closed;
		return false;
	}

    state_ = Opened;
	// emit opened();
	return true;
}

bool Obj::close() {
    if (state_ == Closed)
		return true;

	if (!active()) {
    // not opened
        std::cout << "State is " << state_ << std::endl;
		return false;
	}

    state_ = Closing;
	doClose();
    state_ = Closed;
	// emit closed();
	return true;
}

bool Obj::doOpen() {
  // virtual function call
  std::cout << "virtual function call" << std::endl;
	return false;
}

bool Obj::doClose() {
  // virtual function call
	std::cout << "virtual function call" << std::endl;
	return false;
}
Packet::Result Obj::writeMtuSplit(Packet* packet, size_t mtu) {
    //qDebug() << packet->buf_.size_; // gilgil temp 2021.07.10

    EthHdr* ethHdr = packet->ethHdr_;
    if (ethHdr == nullptr) {
        return Packet::Fail;
    }

    IpHdr* ipHdr = packet->ipHdr_;
    if (ipHdr == nullptr) {
        return Packet::Fail;
    }

    TcpHdr* tcpHdr = packet->tcpHdr_;
    if (tcpHdr == nullptr) {
        return Packet::Fail;
    }

    Buf tcpData = packet->tcpData_;
    if (tcpData.data_ == nullptr) {
        return Packet::Fail;
    }

    Buf backupBuf = packet->buf_;
    if (tempBuf_.data_ == nullptr) {
        tempBuf_.size_ = packet->buf_.size_;
        tempBuf_.data_ = new byte[tempBuf_.size_];
    } else {
        if (tempBuf_.size_ < packet->buf_.size_) {
            delete[] tempBuf_.data_;
            tempBuf_.size_ = packet->buf_.size_;
            tempBuf_.data_ = new byte[tempBuf_.size_];
        }
    }
    memcpy(tempBuf_.data_, packet->buf_.data_, packet->buf_.size_);

    ipHdr->tlen_ = ntohs(mtu);
    ipHdr->checksum_ = htons(IpHdr::calcChecksum(ipHdr));

    byte* tcpDataData = tcpData.data_;

    size_t ipTcpHdrSize = (ipHdr->hlen() + tcpHdr->off()) * 4;
    size_t totalTcpDataSize = packet->buf_.size_ - (sizeof(EthHdr) + ipTcpHdrSize);
    while (true) {
        if (ipTcpHdrSize + totalTcpDataSize <= mtu) break;

        packet->buf_.size_ = sizeof(EthHdr) + mtu;
        size_t onceTcpDataSize = mtu - ipTcpHdrSize;
        tcpHdr->checksum_ = htons(TcpHdr::calcChecksum(ipHdr, tcpHdr));
        write(packet->buf_);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        tcpHdr->seqnum_ = htonl(tcpHdr->seqnum() + onceTcpDataSize); // next seq
        totalTcpDataSize -= onceTcpDataSize;
        memcpy(tcpDataData, tcpDataData + onceTcpDataSize, totalTcpDataSize); // next data
    }
    //qDebug() << "lastTcpDataSize =" << totalTcpDataSize; // gilgil temp 2021.07.10
    ipHdr->tlen_ = ntohs(ipTcpHdrSize + totalTcpDataSize);
    ipHdr->checksum_ = htons(IpHdr::calcChecksum(ipHdr));
    tcpHdr->checksum_ = htons(TcpHdr::calcChecksum(ipHdr, tcpHdr));
    packet->buf_.size_ = sizeof(EthHdr) + ipTcpHdrSize + totalTcpDataSize;
    Packet::Result res = write(packet->buf_); // gilgil temp 2021.07.10

    packet->buf_ = backupBuf;
    memcpy(packet->buf_.data_, tempBuf_.data_, packet->buf_.size_);
    return res;
}
