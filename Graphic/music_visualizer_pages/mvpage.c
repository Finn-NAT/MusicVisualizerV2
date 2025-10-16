#include "mvpage.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SUBPAGES 10

mv_page_t *MusicVisualizerPage = NULL;

mv_page_t *list_subpages[MAX_SUBPAGES] = {
    (mv_page_t*)&BasicMusicVisualizerPage,
};

mv_page_err_code SetSubpage(uint16_t index) {
    if (index >= MAX_SUBPAGES || list_subpages[index] == NULL) {
        printf("Invalid subpage index: %d\n", index);
        return MV_PAGE_RET_FAIL;
    }
    MusicVisualizerPage = list_subpages[index];
    if (!MusicVisualizerPage) return MV_PAGE_RET_FAIL;
    return MV_PAGE_RET_OK;
}
