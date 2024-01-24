from ics import Calendar, Event
from PIL import Image, ImageFont, ImageDraw
from datetime import datetime
import arrow
import calendar
import numpy as np


# choose fonts
font = ImageFont.truetype("arial.ttf", 20)
font_times = ImageFont.truetype("arial.ttf", 15)
font_days = ImageFont.truetype("arial.ttf", 30)


# open the ical file
with open("events.ics", 'r') as file:
    ics_text = file.read()

c = Calendar(ics_text)


def generate_calendar_bitmap(year, month, output_file):
    image_width = 800
    image_height = 480
    cell_size = 115
    cell_height = 65

    # Create a new image
    image = Image.new("RGB", (image_width, image_height), "white")
    draw = ImageDraw.Draw(image)

    # draw lines separating days
    for i in range(7):
        shape = [(cell_size * i, 0), (cell_size * i, image_height)]
        draw.line(shape, fill="black", width=0)

    draw.line([(0, cell_size/2), (image_width, cell_size/2)], fill="black", width=0)


    # Draw days of the week at the top
    weekdays = ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]
    for i, day in enumerate(weekdays):
        draw.text((i * cell_size + cell_size // 5, 10), day, font = font_days, fill="black")

        j = 1
        for event in c.events:
            if event.begin.weekday() == i:
                begin_time = "from: " + event.begin.format("HH:mm")
                end_time = "to: " + event.end.format("HH:mm")

                draw.text((i * cell_size + cell_size // 5, cell_height * j), event.name, font=font, fill="black")
                draw.text((i * cell_size + cell_size // 5, cell_height * j + 30), begin_time, font=font_times, fill="black")
                draw.text((i * cell_size + cell_size // 5, cell_height * j + 55), end_time, font=font_times, fill="black")
                j += 1.4

    # Save the image
    image.save(output_file)


# convert bmp to bytes
def bmp_to_bytes(file_path):
    # Open BMP file
    image = Image.open(file_path)

    # Convert image to numpy array
    np_array = np.array(image)

    # Flatten the array and convert to bytes
    image_buffer = np_array.tobytes()

    return image_buffer


# save image
def save_image_buffer(image_buffer, output_file):
    with open(output_file, 'wb') as file:
        file.write(image_buffer)





def bitmap_to_c_header(bitmap_path, header_path):
    with open(bitmap_path, 'rb') as bitmap_file:
        # Read the entire content of the bitmap file
        bitmap_data = bytearray(bitmap_file.read())

        # Group every 8 bytes into 1 byte and scale to fit uint8_t
        compressed_data = [format(sum(bitmap_data[i:i+8]) // 8, '02X') for i in range(0, len(bitmap_data), 8)]

        # Generate the C header file content
        header_content = f"const uint8_t image_buffer[] = {{0x{', 0x'.join(compressed_data)}}};\n"

        # Write the content to the header file
        with open(header_path, 'w') as header_file:
            header_file.write(header_content)


def convert_png_to_mono_bmp(png_path, bmp_path):
    # Open the PNG image
    img = Image.open(png_path).convert("L")  # Convert to grayscale

    # Save the image in BMP format
    img.save(bmp_path, "BMP")

if __name__ == "__main__":
    year = 2024
    month = 1
    output_file = "calendar_bitmap.png"
    output_bit_file = "image_buffer.dat"
    header_path = "header_file.h"
    bmp_path = "file.bmp"


    generate_calendar_bitmap(year, month, output_file)

    convert_png_to_mono_bmp(output_file, bmp_path)

    image_buffer = bmp_to_bytes(bmp_path)

    bitmap_to_c_header(output_file, header_path)
    save_image_buffer(image_buffer, output_bit_file)
