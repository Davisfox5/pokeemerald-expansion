#ifndef GUARD_CONSTANTS_TMS_HMS_H
#define GUARD_CONSTANTS_TMS_HMS_H

// Apocrypha M04: 100 TMs, twenty per region, each stocked with generic moves that the
// region's own generation introduced. Power is capped at 85; the signature high-power
// moves are deliberately excluded and belong to the regional move tutors instead.
// Regions appear in story order. Dazzling Gleam is not here: it is Gen 6 and the Fairy
// teaching route is a tutor, not a TM.
#define FOREACH_TM(F) \
    /* Johto */ \
    F(PROTECT) \
    F(RETURN) \
    F(FRUSTRATION) \
    F(HIDDEN_POWER) \
    F(SLEEP_TALK) \
    F(ENDURE) \
    F(ATTRACT) \
    F(RAIN_DANCE) \
    F(SUNNY_DAY) \
    F(SANDSTORM) \
    F(SAFEGUARD) \
    F(THIEF) \
    F(ICY_WIND) \
    F(SHADOW_BALL) \
    F(GIGA_DRAIN) \
    F(CRUNCH) \
    F(STEEL_WING) \
    F(ANCIENT_POWER) \
    F(ENCORE) \
    F(FALSE_SWIPE) \
    /* Kanto */ \
    F(TOXIC) \
    F(REST) \
    F(DOUBLE_TEAM) \
    F(REFLECT) \
    F(LIGHT_SCREEN) \
    F(THUNDER_WAVE) \
    F(SWORDS_DANCE) \
    F(AGILITY) \
    F(BODY_SLAM) \
    F(DIG) \
    F(ROCK_SLIDE) \
    F(THUNDER_PUNCH) \
    F(ICE_PUNCH) \
    F(FIRE_PUNCH) \
    F(SWIFT) \
    F(LOW_KICK) \
    F(SEISMIC_TOSS) \
    F(CONFUSE_RAY) \
    F(SCREECH) \
    F(COUNTER) \
    /* Hoenn */ \
    F(FACADE) \
    F(SECRET_POWER) \
    F(ROCK_TOMB) \
    F(AERIAL_ACE) \
    F(BRICK_BREAK) \
    F(KNOCK_OFF) \
    F(SHOCK_WAVE) \
    F(TAUNT) \
    F(WATER_PULSE) \
    F(SIGNAL_BEAM) \
    F(MUD_SHOT) \
    F(CALM_MIND) \
    F(BULK_UP) \
    F(TORMENT) \
    F(HAIL) \
    F(IRON_DEFENSE) \
    F(WILL_O_WISP) \
    F(BULLET_SEED) \
    F(MAGICAL_LEAF) \
    F(AIR_CUTTER) \
    /* Sinnoh */ \
    F(ZEN_HEADBUTT) \
    F(PAYBACK) \
    F(STEALTH_ROCK) \
    F(CHARGE_BEAM) \
    F(POISON_JAB) \
    F(SHADOW_CLAW) \
    F(NASTY_PLOT) \
    F(SEED_BOMB) \
    F(IRON_HEAD) \
    F(DARK_PULSE) \
    F(U_TURN) \
    F(DRAIN_PUNCH) \
    F(ROOST) \
    F(AIR_SLASH) \
    F(TRICK_ROOM) \
    F(NIGHT_SLASH) \
    F(FLASH_CANNON) \
    F(X_SCISSOR) \
    F(AVALANCHE) \
    F(GRASS_KNOT) \
    /* Unova */ \
    F(ROUND) \
    F(BULLDOZE) \
    F(WORK_UP) \
    F(RETALIATE) \
    F(INCINERATE) \
    F(HONE_CLAWS) \
    F(SMACK_DOWN) \
    F(SCALD) \
    F(ACROBATICS) \
    F(PSYSHOCK) \
    F(LOW_SWEEP) \
    F(VENOSHOCK) \
    F(SNARL) \
    F(HEX) \
    F(STRUGGLE_BUG) \
    F(DRAGON_TAIL) \
    F(ELECTROWEB) \
    F(FLAME_CHARGE) \
    F(VOLT_SWITCH) \
    F(DRILL_RUN)

#define FOREACH_HM(F) \
    F(CUT) \
    F(FLY) \
    F(SURF) \
    F(STRENGTH) \
    F(FLASH) \
    F(ROCK_SMASH) \
    F(WATERFALL) \
    F(DIVE)

#define FOREACH_TMHM(F) \
    FOREACH_TM(F) \
    FOREACH_HM(F)

#endif
