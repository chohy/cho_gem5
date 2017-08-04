/**
 * Authors: Haeyoon Cho
 */

#ifndef __MEM_CACHE_TAGS_TAGSHARINGSETASSOC_HH__
#define __MEM_CACHE_TAGS_TAGSHARINGSETASSOC_HH__

#include "mem/cache/base_set_assoc.hh"
#include "params/TagSharingSetAssoc.hh"

class TagSharingSetAssoc : public BaseSetAssoc
{
  protected:
    /** The array of shared tags */
    Addr *sharedTags;

    /** The number of lower (non shared) tag bits */
    const unsigned subBlkSize;
    const unsigned numLowerTagBits;

  public:
    typedef TagSharingSetAssocParmas Params;

    TagSharingSetAssoc(const Params *p);
    virtual ~TagSharingSetAssoc();

    CacheBlk* accessBlock(Addr addr, bool is_secure, Cycles &lat);
    CacheBlk* findVictim(Addr addr);
    void insertBlock(PacketPtr pkt, BlkType *blk) override;
    void invalidate(CacheBlk *blk);
    //void invalidateSet(Addr addr);

    bool isShared(Addr addr) {
        return (extractSharedTag(addr)==
                sharedTags[extractSharedTag(addr)]) ? true : false;
    }

    Addr extractSharedTag(Addr addr) {
        return BaseSetAssoc::extractTag(addr) >> numLowerTagBits;
    }

}

#endif // __MEM_CACHE_TAGS_TAGSHARINGSETASSOC_HH__
