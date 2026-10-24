#ifndef GUARD_OLIVINE_VIEW_H
#define GUARD_OLIVINE_VIEW_H
bool32 OlivineViewActive(void);
void OlivineViewForget(void);
void OlivineViewBegin(void);
void OlivineViewReleaseSlice(u16 offset, bool32 vertical);
bool32 OlivineViewMetatile(u16 offset, s32 x, s32 y, u16 *tiles, s32 *layer);
#endif
