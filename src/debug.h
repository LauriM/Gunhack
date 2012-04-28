#define _ASSERT_RANGE(p_value,p_max) assert(p_value >= 0); assert(p_value < p_max);

#define  ASSERT_HEIGHT(p_height)              _ASSERT_RANGE(p_height,MAP_MAX_HEIGHT);
#define  ASSERT_ROOM(p_depth)                 _ASSERT_RANGE(p_depth,WORLD_ROOM_COUNT);
#define  ASSERT_WIDTH(p_width)                _ASSERT_RANGE(p_width,MAP_MAX_WIDTH);
#define  ASSERT_ITEM_MAX_COUNT(p_count)       _ASSERT_RANGE(p_count,ITEM_MAX_COUNT);
#define  ASSERT_ITEM_VIS_MAX_COUNT(p_count)   _ASSERT_RANGE(p_count,ITEM_MAX_COUNT)
#define  ASSERT_ITEM_INFO_MAX_COUNT(p_count)  _ASSERT_RANGE(p_count,ITEM_COUNT);
#define  ASSERT_TILE_TYPE(p_type)             _ASSERT_RANGE(p_type,TILE_COUNT);
