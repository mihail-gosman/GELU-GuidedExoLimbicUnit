import pygame
import sys

pygame.init()

WIDTH, HEIGHT = 600, 400
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
SLIDER_COLOR = (50, 50, 50)
BUTTON_COLOR = (100, 100, 100)
FONT_SIZE = 20

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Slide Buttons")

num_sliders = 3
slider_width = 20
slider_height = 200
slider_x = WIDTH // 4 - slider_width // 2
slider_spacing = WIDTH // 4

button_width = 30
button_height = 50
button_x = slider_x - button_width // 2
button_spacing = slider_spacing

button_positions = [0] * num_sliders
selected_button = 0

commands_lists = [[] for _ in range(num_sliders)]

font = pygame.font.Font(None, FONT_SIZE)

instructions = "Press 1, 2, 3 to select a slide. Use Up/Down arrows to move the selected slide."
text = font.render(instructions, True, BLACK)
text_rect = text.get_rect(center=(WIDTH // 2, 70))

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_1:
                selected_button = 0
            elif event.key == pygame.K_2:
                selected_button = 1
            elif event.key == pygame.K_3:
                selected_button = 2
            elif event.key == pygame.K_UP:
                button_positions[selected_button] = max(0, button_positions[selected_button] - 0.1)
                
                commands_lists[selected_button].append(f"move servo_{selected_button + 1} {round(button_positions[selected_button], 2)}")
                print(commands_lists[selected_button])
            elif event.key == pygame.K_DOWN:
                button_positions[selected_button] = min(1, button_positions[selected_button] + 0.1)
                
                commands_lists[selected_button].append(f"move servo_{selected_button + 1} {round(button_positions[selected_button], 2)}")
                print(commands_lists[selected_button])

    screen.fill(WHITE)

    for i in range(num_sliders):
        pygame.draw.rect(screen, SLIDER_COLOR, (slider_x + i * slider_spacing, 100, slider_width, slider_height))
        button_y = int(100 + (slider_height - button_height) * button_positions[i])
        pygame.draw.rect(screen, BUTTON_COLOR, (button_x + i * button_spacing, button_y, button_width, button_height))

    screen.blit(text, text_rect)

    pygame.display.flip()

pygame.quit()
sys.exit()

