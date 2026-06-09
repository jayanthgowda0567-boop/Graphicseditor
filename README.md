# 2D Graphics Editor (C)

This is a simple menu-driven 2D graphics editor implemented in C.

- Canvas is a 2D char array initially filled with `_`.
- Shapes are drawn using `*`.
- Supported shapes: line, rectangle, circle, triangle.
- You can add, delete, modify shapes and display or save the canvas as an ASCII file.

Build
-----
Requires a C compiler (gcc/clang).

From the project directory:

```sh
make
```

Run
---

```sh
./editor [width height]
# defaults to 60x20 when no args provided
```

Usage notes
-----------
- Coordinates: x ranges from 0..width-1 (across), y ranges 0..height-1 (down).
- Use the interactive menu to add/list/delete/modify shapes.

Submitting to Moodle / GitHub
----------------------------
1. Create a new GitHub repository (private or public) and push this folder. Example commands:

```sh
cd "C Programs/c project"
git init
git add graphics_editor
git commit -m "Add 2D graphics editor"
# create repo on GitHub via web or gh CLI, then push:
git remote add origin https://github.com/<your-username>/<repo>.git
git branch -M main
git push -u origin main
```

2. Daily pushes are required — make a small commit each day and push to GitHub.

3. Submit the GitHub repository link to Moodle using the assignment's online text field.

Good luck!
