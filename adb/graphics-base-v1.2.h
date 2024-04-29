#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        HAL_HDR_HDR10_PLUS = 4,
    } android_hdr_v1_2_t;

    typedef enum {
        HAL_DATASPACE_DISPLAY_BT2020 = 142999552 /* STANDARD_BT2020 | TRANSFER_SRGB | RANGE_FULL */,
        HAL_DATASPACE_DYNAMIC_DEPTH = 4098 /* 0x1002 */,
        HAL_DATASPACE_JPEG_APP_SEGMENTS = 4099 /* 0x1003 */,
        HAL_DATASPACE_HEIF = 4100 /* 0x1004 */,
    } android_dataspace_v1_2_t;

    typedef enum {
        HAL_COLOR_MODE_DISPLAY_BT2020 = 13,
    } android_color_mode_v1_2_t;

    typedef enum {
        HAL_PIXEL_FORMAT_HSV_888 = 55 /* 0x37 */,
    } android_pixel_format_v1_2_t;

#ifdef __cplusplus
}
#endif