#!/usr/bin/env python3

import pygame

# Cell side (pixels)
cellSide = 30

# Window background color
backgroundColor = (0, 0, 0)

# Grid color
gridColor = (255, 255, 255)

# Window size
screen_width = cellSide * 16 * 3
screen_height = cellSide * 9 * 3
screen = pygame.display.set_mode((screen_width, screen_height))

# Window name
pygame.display.set_caption('Fluid Motion')

# Fill window color
screen.fill(backgroundColor)
pygame.display.flip()

# Loop variable
running = True


# Cell class
class Cell:
    def __init__(self):
        self.side = cellSide

# Wall class
class Wall(Cell):
    def __init__(self):
        super(self)
        self.color = (255, 0, 0)
        

# Function to draw the grid
def draw_grid():
    for x in range(0, screen_width, cellSide):
        pygame.draw.line(screen, gridColor, (x, 0), (x, screen_height))
    for y in range(0, screen_height, cellSide):
        pygame.draw.line(screen, gridColor, (0, y), (screen_width, y))


# Simulation loop
while running:
    # Event loop
    for event in pygame.event.get():
        # Check for QUIT event
        if event.type == pygame.QUIT:
            running = False

    # Draw the grid
    screen.fill(backgroundColor)
    draw_grid()
    pygame.display.flip()


# Quit Pygame
pygame.quit()