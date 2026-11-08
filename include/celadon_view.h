#ifndef GUARD_CELADON_VIEW_H
#define GUARD_CELADON_VIEW_H
bool32 CeladonViewActive(void);
void CeladonViewForget(void);
void CeladonViewBegin(void);
void CeladonViewStep(void);
bool32 CeladonViewMetatile(u16 offset, s32 x, s32 y, u16 *tiles, s32 *layer);
const u32 *CeladonViewAnimFrame(u16 timer, u16 *slot, u32 *size);
#endif
