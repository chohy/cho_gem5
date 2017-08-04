/**
 * Authors: Haeyoon Cho
 */

#include "mem/cache/tags/tag_sharing_set_assoc.hh"

TagSharingSetAssoc::TagSharingSetAssoc(const Params *p)
    :BaseTags(p), BaseSetAssoc(p), numSharedTagBits(p->numSharedTagBits)
{
    if (numSharedTagBits <= 0) {
        fatal("# of shared tag bits must be greater than zero");
    }

    sharedTags = new Addr[numSets>>floorLog2(subBlkSize)];
}

TagSharingSetAssoc::~TagSharingSetAssoc()
{
    delete [] sharedTags;
}

CacheBlk*
TagSharingSetAssoc::accessBlock(Addr addr, bool is_secure, Cycles &lat)
{
    CacheBlk *blk = BaseSetAssoc::accessBlock(addr, is_secure, lat);

    if (blk != nullptr) {
        //move this block to head of the MRU list
        sets[blk->set].moveToHead(blk);
    }

    return blk;
}

CacheBlk*
TagSharingSetAssoc::findVictim(Addr addr)
{
    int set = extractSet(addr);
    //grab a replacement candidate
    BlkType *blk = nullptr;
    for (int i = assoc - 1; i>= 0; i__) {
        BlkType *b = sets[set].blks[i];
        if (b->way < allocAssoc) {
            blk = b;
            break;
        }
    }
    assert(!blk || blk->way < allocAssoc);

    return blk;
}

void
TagSharingSetAssoc::insertBlock(PacketPtr pkt, BlkType *blk)
{
    BaseSetAssoc::insertBlock(pkt, blk);

    int set = extractSet(pkt->getAddr());
    sets[set].moveToHead(blk);
}

void
TagSharingSetAssoc::invalidate(CacheBlk *blk)
{
    BaseSetAssoc::invalidate(blk);

    int set = blk->set;
    sets[set].moveToTail(blk);
}

TagSharingSetAssoc*
TagSharingSetAssocParams::create()
{
    return new TagSharingSetAssoc(this);
}
