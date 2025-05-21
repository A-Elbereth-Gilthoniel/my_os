DEF_CMD("color",           \
    char* arg = get_next_word(&cmd);
    if (strcmp(arg, "black") == 0)
        monitor->color = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
    else if (strcmp(arg, "grey") == 0)
        monitor->color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_LIGHT_RED);

    free(arg);
)
