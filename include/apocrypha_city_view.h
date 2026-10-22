#ifndef GUARD_APOCRYPHA_CITY_VIEW_H
#define GUARD_APOCRYPHA_CITY_VIEW_H

// Cities too detailed for one tileset pair are split into several ordinary
// Porymap sections. This view streams each section's own native tiles into
// VRAM as the camera moves, so neighbouring sections draw with their own art.
// It is inactive on every other map.
bool32 CityView_IsActive(void);
void CityView_Forget(void);
void CityView_Begin(void);
bool32 CityView_DrawMetatile(u16 offset, s32 x, s32 y, u16 *tiles, s32 *layerType);

#endif // GUARD_APOCRYPHA_CITY_VIEW_H
