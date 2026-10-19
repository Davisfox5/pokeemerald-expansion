#ifndef GUARD_GOLDENROD_VIEW_H
#define GUARD_GOLDENROD_VIEW_H
bool32 GoldenrodViewActive(void);
void GoldenrodViewForget(void);
void GoldenrodViewBegin(void);
bool32 GoldenrodViewMetatile(u16 offset, s32 x, s32 y, u16 *tiles, s32 *layer);
#endif
