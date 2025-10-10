#include "subpage.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SUBPAGES 10

subpage_t *MusicVisualizerPage = NULL;

subpage_t *list_subpages[MAX_SUBPAGES] = {
    &MusicVisualizerTestPage,
};

void SetSubpage(uint16_t index) {
    if (index >= MAX_SUBPAGES || list_subpages[index] == NULL) {
        printf("Invalid subpage index: %d\n", index);
        return;
    }
    MusicVisualizerPage = malloc(list_subpages[index]->subpage_size);
    if (!MusicVisualizerPage) return;
    printf("BUG %d\n", list_subpages[index]->subpage_size);
    memcpy(MusicVisualizerPage, list_subpages[index], list_subpages[index]->subpage_size);
}