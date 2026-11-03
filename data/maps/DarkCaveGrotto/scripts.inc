DarkCaveGrotto_MapScripts::
    map_script MAP_SCRIPT_ON_RESUME, DarkCaveGrotto_OnResume
    .byte 0

DarkCaveGrotto_OnResume:
    setdivewarp MAP_DARK_CAVE_UNDERWATER, 28, 34
    end
