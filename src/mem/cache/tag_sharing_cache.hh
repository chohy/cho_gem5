/**
 * Authors: Haeyoon Cho
 */

#include "mem/cache/cache.hh"
#include "params/TagSharingCache.hh"

class TagSharingCache : public Cache
{
  protected:
    bool access(PacketPtr pkt, Cacheblk *&blk,
                Cycles &lat, PacketList &writebacks) override;

    CacheBlk *handleFill(PacketPtr pkt, CacheBlk *blk,
                         PacketList &writebacks, bool allocate) override;

  public:

    TagSharingCache(const TagSharingCacheParams *p);
    virtual ~TagSharingCache();
}
