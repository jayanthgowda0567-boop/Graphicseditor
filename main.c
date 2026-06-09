/*
 * Simple menu-driven 2D Graphics Editor in C
 * Canvas uses a 2D char array filled with '_' and draws with '*'
 * Supports: line, rectangle, circle, triangle - add/delete/modify/display
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SHAPES 200
#define MAX_W 120
#define MAX_H 60

typedef enum {SH_LINE, SH_RECT, SH_CIRCLE, SH_TRIANGLE} ShapeType;

typedef struct {
    int id;
    ShapeType type;
    // parameters used depending on type
    int x1, y1, x2, y2, x3, y3, r;
} Shape;

static Shape shapes[MAX_SHAPES];
static int shapes_count = 0;

static int width = 60;
static int height = 20;
static char canvas[MAX_H][MAX_W];

void init_canvas() {
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            canvas[i][j] = '_';
}

void clear_canvas() { init_canvas(); }

void print_canvas() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j)
            putchar(canvas[i][j]);
        putchar('\n');
    }
}

static void set_pixel(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        canvas[y][x] = '*';
}

// Bresenham's line
void draw_line_on_canvas(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (1) {
        set_pixel(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_rectangle_on_canvas(int x1, int y1, int x2, int y2) {
    int left = x1 < x2 ? x1 : x2;
    int right = x1 < x2 ? x2 : x1;
    int top = y1 < y2 ? y1 : y2;
    int bottom = y1 < y2 ? y2 : y1;
    for (int x = left; x <= right; ++x) { set_pixel(x, top); set_pixel(x, bottom); }
    for (int y = top; y <= bottom; ++y) { set_pixel(left, y); set_pixel(right, y); }
}

void draw_circle_on_canvas(int cx, int cy, int r) {
    if (r < 0) return;
    int x0 = cx, y0 = cy;
    for (int y = cy - r; y <= cy + r; ++y) {
        for (int x = cx - r; x <= cx + r; ++x) {
            int dx = x - x0, dy = y - y0;
            if (dx*dx + dy*dy <= r*r + r) // +r for slight thickness
                set_pixel(x, y);
        }
    }
}

void draw_triangle_on_canvas(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line_on_canvas(x1,y1,x2,y2);
    draw_line_on_canvas(x2,y2,x3,y3);
    draw_line_on_canvas(x3,y3,x1,y1);
}

void redraw_all() {
    clear_canvas();
    for (int i = 0; i < shapes_count; ++i) {
        Shape *s = &shapes[i];
        switch (s->type) {
            case SH_LINE: draw_line_on_canvas(s->x1,s->y1,s->x2,s->y2); break;
            case SH_RECT: draw_rectangle_on_canvas(s->x1,s->y1,s->x2,s->y2); break;
            case SH_CIRCLE: draw_circle_on_canvas(s->x1,s->y1,s->r); break;
            case SH_TRIANGLE: draw_triangle_on_canvas(s->x1,s->y1,s->x2,s->y2,s->x3,s->y3); break;
        }
    }
}

int next_id() {
    static int id = 1; return id++;
}

void add_shape() {
    if (shapes_count >= MAX_SHAPES) { printf("Shape limit reached.\n"); return; }
    printf("Choose shape type: 1=Line 2=Rectangle 3=Circle 4=Triangle: ");
    int t; if (scanf("%d", &t) != 1) return; 
    Shape s; s.id = next_id(); s.r = 0; s.x1=s.y1=s.x2=s.y2=s.x3=s.y3=0;
    if (t == 1) {
        s.type = SH_LINE; printf("Enter x1 y1 x2 y2: "); scanf("%d %d %d %d", &s.x1,&s.y1,&s.x2,&s.y2);
    } else if (t == 2) {
        s.type = SH_RECT; printf("Enter x1 y1 x2 y2 (two opposite corners): "); scanf("%d %d %d %d", &s.x1,&s.y1,&s.x2,&s.y2);
    } else if (t == 3) {
        s.type = SH_CIRCLE; printf("Enter centerX centerY radius: "); scanf("%d %d %d", &s.x1,&s.y1,&s.r);
    } else if (t == 4) {
        s.type = SH_TRIANGLE; printf("Enter x1 y1 x2 y2 x3 y3: "); scanf("%d %d %d %d %d %d", &s.x1,&s.y1,&s.x2,&s.y2,&s.x3,&s.y3);
    } else { printf("Invalid type.\n"); return; }
    shapes[shapes_count++] = s;
    redraw_all();
    printf("Added shape id=%d\n", s.id);
}

void list_shapes() {
    if (shapes_count == 0) { printf("No shapes.\n"); return; }
    for (int i = 0; i < shapes_count; ++i) {
        Shape *s = &shapes[i];
        printf("id=%d ", s->id);
        switch (s->type) {
            case SH_LINE: printf("Line (%d,%d)-(%d,%d)\n", s->x1,s->y1,s->x2,s->y2); break;
            case SH_RECT: printf("Rect (%d,%d)-(%d,%d)\n", s->x1,s->y1,s->x2,s->y2); break;
            case SH_CIRCLE: printf("Circle center=(%d,%d) r=%d\n", s->x1,s->y1,s->r); break;
            case SH_TRIANGLE: printf("Triangle (%d,%d) (%d,%d) (%d,%d)\n", s->x1,s->y1,s->x2,s->y2,s->x3,s->y3); break;
        }
    }
}

int find_index_by_id(int id) {
    for (int i = 0; i < shapes_count; ++i) if (shapes[i].id == id) return i;
    return -1;
}

void delete_shape() {
    printf("Enter id to delete: "); int id; if (scanf("%d", &id) != 1) return;
    int idx = find_index_by_id(id); if (idx < 0) { printf("Not found.\n"); return; }
    for (int i = idx; i+1 < shapes_count; ++i) shapes[i] = shapes[i+1];
    shapes_count--;
    redraw_all();
    printf("Deleted id=%d\n", id);
}

void modify_shape() {
    printf("Enter id to modify: "); int id; if (scanf("%d", &id) != 1) return;
    int idx = find_index_by_id(id); if (idx < 0) { printf("Not found.\n"); return; }
    Shape *s = &shapes[idx];
    if (s->type == SH_LINE) { printf("Enter new x1 y1 x2 y2: "); scanf("%d %d %d %d", &s->x1,&s->y1,&s->x2,&s->y2); }
    else if (s->type == SH_RECT) { printf("Enter new x1 y1 x2 y2: "); scanf("%d %d %d %d", &s->x1,&s->y1,&s->x2,&s->y2); }
    else if (s->type == SH_CIRCLE) { printf("Enter new centerX centerY radius: "); scanf("%d %d %d", &s->x1,&s->y1,&s->r); }
    else if (s->type == SH_TRIANGLE) { printf("Enter new x1 y1 x2 y2 x3 y3: "); scanf("%d %d %d %d %d %d", &s->x1,&s->y1,&s->x2,&s->y2,&s->x3,&s->y3); }
    redraw_all();
    printf("Modified id=%d\n", id);
}

void set_canvas_size() {
    printf("Enter width height (max %d x %d): ", MAX_W, MAX_H);
    int w,h; if (scanf("%d %d", &w, &h) != 2) return;
    if (w < 5 || w > MAX_W || h < 5 || h > MAX_H) { printf("Invalid size.\n"); return; }
    width = w; height = h; init_canvas(); redraw_all();
}

void save_ascii(const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) { printf("Failed to open file.\n"); return; }
    for (int i = 0; i < height; ++i) {
        fwrite(canvas[i], 1, width, f);
        fputc('\n', f);
    }
    fclose(f);
    printf("Saved to %s\n", path);
}

void menu_loop() {
    int running = 1;
    while (running) {
        printf("\n--- 2D Graphics Editor ---\n");
        printf("1) Display canvas\n2) Add shape\n3) List shapes\n4) Delete shape\n5) Modify shape\n6) Set canvas size\n7) Save ASCII file\n8) Clear all\n9) Exit\nChoose: ");
        int c; if (scanf("%d", &c) != 1) break;
        switch (c) {
            case 1: print_canvas(); break;
            case 2: add_shape(); break;
            case 3: list_shapes(); break;
            case 4: delete_shape(); break;
            case 5: modify_shape(); break;
            case 6: set_canvas_size(); break;
            case 7: { char path[256]; printf("Enter path: "); scanf("%255s", path); save_ascii(path); } break;
            case 8: shapes_count = 0; clear_canvas(); printf("Cleared.\n"); break;
            case 9: running = 0; break;
            default: printf("Invalid.\n"); break;
        }
    }
}

int main(int argc, char **argv) {
    if (argc >= 3) {
        width = atoi(argv[1]); height = atoi(argv[2]);
        if (width < 5 || height < 5) { width = 60; height = 20; }
    }
    init_canvas();
    printf("2D Graphics Editor (text). Canvas %dx%d\n", width, height);
    printf("Coordinates: x in [0..%d], y in [0..%d] (x across, y down)\n", width-1, height-1);
    menu_loop();
    return 0;
}
