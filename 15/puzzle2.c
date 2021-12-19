#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct point {
    int x, y;
} point_t;

#define POINT(x,y) ((point_t){(x), (y)})
#define NOWHERE POINT(-1, -1)

typedef struct step {
    point_t pos;
    point_t prev_step;
    int min_cost;
} step_t;

#define INVALID_STEP ((step_t){NOWHERE, NOWHERE, INT_MAX})

typedef struct queue_link {
    step_t step;
    struct queue_link *prev;
    struct queue_link *next;
} queue_t;

typedef struct cavern_search {
    int width, height;
    int *map;
    queue_t *queue_array;
    queue_t top_sentinel;
    queue_t bottom_sentinel;
    bool *found_array;
} cavern_search_t;

int *read_map(FILE *input, int *width_p, int *height_p)
{
    int width = 0;
    int height = 0;

    int map_buf_size = 4000;
    int *map = malloc(sizeof(int) * map_buf_size);

    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_len = 0;

    while ((line_len = getline(&line_buf, &line_buf_size, input)) >= 0) {
        if (line_buf[line_len-1] == '\n') {
            --line_len;
        }
        if (width == 0) {
            width = line_len;
        } else if (line_len != width) {
            fprintf(stderr, "Inconsistent width: %d or %lld?\n", width, (long long)line_len);
            return NULL;
        }
        ++height;
        if (width * height > map_buf_size) {
            map_buf_size *= 2;
            map = realloc(map, sizeof(int) * map_buf_size);
        }
        int y = height - 1;
        for (int x = 0; x < width; ++x) {
            map[y * width + x] = line_buf[x] - '0';
        }
    }

    if (width_p != NULL) *width_p = width;
    if (height_p != NULL) *height_p = height;

    return realloc(map, sizeof(int) * width * height);
}

void complete_map(int *map, int *tile, int tile_width, int tile_height)
{
    int full_width = 5 * tile_width;
    int full_height = 5 * tile_height;

    for (int y = 0; y < full_height; ++y) {
        int tile_y = y % tile_height;
        for (int x = 0; x < full_width; ++x) {
            int tile_x = x % tile_width;
            int delta = x / tile_width + y / tile_height;
            int tile_val = tile[tile_y * tile_width + tile_x];
            int new_val = (tile_val + delta - 1) % 9 + 1;
            map[y * full_width + x] = new_val;
        }
    }
}

cavern_search_t *init_cavern(FILE *input)
{
    cavern_search_t *cavern = malloc(sizeof(cavern_search_t));
    int tile_width, tile_height;
    int *tile = read_map(input, &tile_width, &tile_height);
    if (tile == NULL) {
        free(cavern);
        return NULL;
    }

    cavern->height = 5 * tile_height;
    cavern->width = 5 * tile_width;
    cavern->map = malloc(sizeof(int) * cavern->height * cavern->width);
    complete_map(cavern->map, tile, tile_width, tile_height);
    free(tile);

    int w = cavern->width;
    int h = cavern->height;

    cavern->top_sentinel = (queue_t){INVALID_STEP, NULL, NULL};
    cavern->bottom_sentinel = (queue_t){INVALID_STEP, NULL, NULL};
    cavern->queue_array = malloc(sizeof(queue_t) * cavern->width * cavern->height);
    for (int y = 0; y < cavern->height; ++y) {
        for (int x = 0; x < cavern->width; ++x) {
            queue_t *link = &cavern->queue_array[y * w + x];
            link->step = (step_t){{x, y}, NOWHERE, INT_MAX};
            link->prev = link - 1;
            link->next = link + 1;
        }
    }
    cavern->queue_array[0].prev = &cavern->top_sentinel;
    cavern->top_sentinel.next = &cavern->queue_array[0];
    cavern->queue_array[0].step.min_cost = 0;
    cavern->queue_array[w * h - 1].next = &cavern->bottom_sentinel;
    cavern->bottom_sentinel.prev = &cavern->queue_array[w * h - 1];

    cavern->found_array = calloc(w * h, sizeof(bool));

    return cavern;
}

void free_cavern(cavern_search_t *cavern)
{
    free(cavern->queue_array);
    free(cavern->map);
    free(cavern);
}

void reinsert_queue_entry(cavern_search_t *cavern, queue_t *entry)
{
    // extract
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    // reinsert, probably near the front
    for (queue_t *other = cavern->top_sentinel.next; other != NULL; other = other->next) {
        if (other->step.min_cost > entry->step.min_cost) {
            other->prev->next = entry;
            entry->prev = other->prev;
            entry->next = other;
            other->prev = entry;
            return;
        }
    }
}

void set_reachable_from(cavern_search_t *cavern, queue_t *from_entry, point_t pos)
{
    int w = cavern->width;
    int h = cavern->height;   
    if (pos.x < 0 || pos.y < 0 || pos.x >= w || pos.y >= h)
        return; // out of bounds

    int idx = pos.y * w + pos.x;
    if (cavern->found_array[idx])
        return; // already found best route here

    int new_cost = from_entry->step.min_cost + cavern->map[idx];
    queue_t *entry = &cavern->queue_array[idx];
    if (new_cost < entry->step.min_cost) {
        // better than previously thought
        entry->step.min_cost = new_cost;
        entry->step.prev_step = from_entry->step.pos;
        reinsert_queue_entry(cavern, entry);
    }
    // else, there's nothing to do: we already had a better path to pos
}

void find_safest_path(cavern_search_t *cavern)
{
    // Dijkstra search based on what I understood from that computerphile video
    // using data structures that may or may not be suitable
    int w = cavern->width;
    int h = cavern->height;

    for (;;) {
        queue_t *this_entry = cavern->top_sentinel.next;
        point_t pos = this_entry->step.pos;
        // unlink
        this_entry->prev->next = this_entry->next;
        this_entry->next->prev = this_entry->prev;
        cavern->found_array[pos.y * h + pos.x] = true;
        // are we at the end?
        if (pos.x == w-1 && pos.y == h-1) {
            return;
        }
        // go through all the neighbours
        set_reachable_from(cavern, this_entry, POINT(pos.x+1, pos.y));
        set_reachable_from(cavern, this_entry, POINT(pos.x-1, pos.y));
        set_reachable_from(cavern, this_entry, POINT(pos.x, pos.y+1));
        set_reachable_from(cavern, this_entry, POINT(pos.x, pos.y-1));
    }
}

// void print_path(cavern_search_t *cavern)
// {
//     int w = cavern->width;
//     int h = cavern->height;

//     // over allocate for simplicity
//     point_t *steps = malloc(w*h*sizeof(point_t));
//     steps[0] = POINT(w - 1, h - 1);
//     int n = 1;
//     while (steps[n-1].x != 0 || steps[n-1].y != 0) {
//         int x = steps[n-1].x;
//         int y = steps[n-1].y;
//         steps[n++] = cavern->queue_array[y * w + x].step.prev_step;
//     }

//     for (int i = n-1; i >= 0; --i) {
//         printf("%d, %d\n", steps[i].x, steps[i].y);
//     }
// }

void print_map(cavern_search_t *cavern)
{
    for (int y = 0; y < cavern->height; ++y) {
        for (int x = 0; x < cavern->width; ++x) {
            putchar('0' + cavern->map[y * cavern->width + x]);
        }
        putchar('\n');
    }
}

int main ()
{
    cavern_search_t *cavern = init_cavern(stdin);
    if (cavern == NULL)
        return 1;

    // print_map(cavern);

    find_safest_path(cavern);
    // print_path(cavern);
    
    int cost = cavern->queue_array[cavern->width * cavern->height - 1].step.min_cost;
    printf("%d\n", cost);

    free_cavern(cavern);
    return 0;
}
