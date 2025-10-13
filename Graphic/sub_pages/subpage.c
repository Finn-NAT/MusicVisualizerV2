#include "subpage.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SUBPAGES 10

subpage_t *MusicVisualizerPage = NULL;

subpage_t *list_subpages[MAX_SUBPAGES] = {
    (subpage_t*)&MusicVisualizerTestPage,
};

sub_page_err_code SetSubpage(uint16_t index) {
    if (index >= MAX_SUBPAGES || list_subpages[index] == NULL) {
        printf("Invalid subpage index: %d\n", index);
        return SUB_PAGE_RET_FAIL;
    }
    MusicVisualizerPage = list_subpages[index];
    if (!MusicVisualizerPage) return SUB_PAGE_RET_FAIL;
    return SUB_PAGE_RET_OK;
}
