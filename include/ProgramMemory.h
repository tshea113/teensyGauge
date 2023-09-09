#pragma once

#include <avr/pgmspace.h>

const uint8_t miata_logo[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xE0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7F, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x1F, 0xF0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0xFC, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xC0, 0x0F, 0xF0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF,
    0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x1F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF8, 0x00, 0x1F, 0x00, 0x01, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x00,
    0x1F, 0x00, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x08, 0xFF, 0xF0, 0x00, 0x3E, 0x00, 0x1F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF,
    0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xE0, 0x00, 0x3C, 0x00, 0x7F, 0xFF, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xC0, 0x00, 0x7C,
    0x01, 0xFC, 0x7F, 0x80, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x3F, 0xFF, 0x00, 0x00, 0xF8, 0x03, 0xF8, 0x7F, 0x80, 0x00, 0x00, 0x7F, 0xF0, 0x00, 0x03, 0xFF, 0xFF, 0xFF,
    0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x01, 0xF0, 0x07, 0xF0, 0x7F, 0x80, 0x00, 0x00, 0x7F,
    0xF0, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF8, 0x00, 0x01, 0xF0, 0x1F,
    0xC0, 0xFF, 0x80, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7F, 0xF8, 0x00, 0x03, 0xF0, 0x7F, 0x80, 0xFF, 0x00, 0x7C, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFC,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF8, 0x00, 0x07, 0xE1, 0xFE, 0x01, 0xFF, 0x07, 0xFE, 0x00, 0x00, 0x78,
    0x00, 0x00, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x07, 0xC3, 0xFC, 0x01,
    0xFF, 0x0F, 0xFF, 0x80, 0x00, 0x10, 0x00, 0x08, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
    0x80, 0x00, 0x0F, 0xC7, 0xF8, 0x03, 0xFF, 0x1F, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x07, 0xF8, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xCF, 0xF0, 0x03, 0xFF, 0x3F, 0xFF, 0xE0, 0x00, 0x00, 0x00,
    0x7F, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xC0, 0x03, 0xFE,
    0x3F, 0xFF, 0xF0, 0x00, 0x60, 0x00, 0xFF, 0xFB, 0xF8, 0x07, 0xF8, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3F, 0xFF, 0x80, 0x07, 0xFE, 0x7C, 0x3F, 0xF0, 0x03, 0xF0, 0x01, 0xFF, 0xE3, 0xF8, 0x07, 0xF8, 0x07, 0xFE,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x07, 0xFF, 0xFC, 0x3F, 0xF0, 0x03, 0xF0, 0x03, 0xFF,
    0xC3, 0xFC, 0x07, 0xF8, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x07, 0xFF, 0xF8,
    0x3F, 0xF8, 0x01, 0xF8, 0x07, 0xFF, 0x81, 0xFC, 0x07, 0xFC, 0x1F, 0xFC, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xF8, 0x1F, 0xF8, 0x01, 0xF8, 0x0F, 0xFF, 0x01, 0xFE, 0x03, 0xFC, 0x3F, 0xF0, 0x0F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xF8, 0x00, 0x07, 0xFF, 0xF8, 0x1F, 0xF8, 0x01, 0xFC, 0x1F, 0xFE, 0x01,
    0xFE, 0x03, 0xFC, 0x7F, 0xE0, 0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xF0, 0x00, 0x07, 0xFF, 0xF0, 0x3F,
    0xF8, 0x01, 0xFC, 0x3F, 0xFC, 0x01, 0xFE, 0x03, 0xFF, 0xFF, 0xC0, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF,
    0xE0, 0x00, 0x07, 0xFF, 0xF0, 0x3F, 0xF8, 0x01, 0xFF, 0xFF, 0xF8, 0x01, 0xFF, 0x03, 0xFF, 0xFF, 0xC0, 0x07, 0xF8,
    0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xE0, 0x00, 0x03, 0xFF, 0xF0, 0x3F, 0xF8, 0x01, 0xFF, 0xFF, 0xE0, 0x01, 0xFF,
    0x83, 0xFF, 0xFF, 0xC0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xC0, 0x00, 0x03, 0xFF, 0xE0, 0x3F, 0xF8,
    0x01, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xC1, 0xFF, 0xFF, 0x80, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0x80,
    0x00, 0x00, 0xFF, 0xC0, 0x3F, 0xF8, 0x07, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xF1, 0xFF, 0xFF, 0x80, 0x03, 0xFC, 0x00,
    0x00, 0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x03, 0x80, 0x1F, 0xF8, 0x1C, 0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0xFF,
    0xFF, 0xFF, 0x80, 0x03, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x7C,
    0xFF, 0xFF, 0xF8, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x3F, 0xF8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFC, 0x7F, 0xFF, 0xFE, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xF0, 0x00,
    0x00, 0x00, 0x7F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xE0, 0x0F, 0x7F, 0xFC, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF8, 0x00,
    0x1F, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xF8, 0x10, 0x1F, 0xFC, 0x00, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0x00, 0x7F, 0xC7, 0xFF, 0xFF, 0xE0, 0x07, 0xFC, 0x0F, 0x00,
    0x00, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xE0, 0x00, 0x03, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x1F,
    0xFF, 0xC0, 0x00, 0x7F, 0x8C, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x80, 0x00, 0x00,
    0xFF, 0xD8, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t fan_icon[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x18, 0x00, 0x0F, 0xFC, 0x3C, 0x00, 0x1F, 0xFE, 0x3E, 0x00, 0x1F,
    0xFC, 0x7F, 0x00, 0x3F, 0xFC, 0x7F, 0x80, 0x3F, 0xF8, 0x7F, 0x80, 0x3F, 0xF0, 0x7F, 0xC0, 0x7F, 0xC0, 0x7F, 0xE0,
    0x7F, 0x80, 0x7F, 0xF0, 0x3F, 0x00, 0x7F, 0xF0, 0x0C, 0x00, 0x3F, 0xF1, 0x80, 0x00, 0x1F, 0xE3, 0xE0, 0x00, 0x07,
    0xE7, 0xE0, 0x00, 0x00, 0xE7, 0xE0, 0x00, 0x00, 0x07, 0xE4, 0x00, 0x00, 0x07, 0xE7, 0xC0, 0x00, 0x01, 0xC7, 0xF0,
    0x00, 0x00, 0x0F, 0xFC, 0x00, 0x3C, 0x1F, 0xFC, 0x00, 0x7F, 0x0F, 0xFE, 0x01, 0xFF, 0x07, 0xFE, 0x03, 0xFE, 0x07,
    0xFE, 0x07, 0xFE, 0x03, 0xFE, 0x0F, 0xFE, 0x01, 0xFE, 0x1F, 0xFC, 0x00, 0xFE, 0x3F, 0xFC, 0x00, 0x7C, 0x3F, 0xF8,
    0x00, 0x3C, 0x1F, 0xF8, 0x00, 0x18, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t cold_icon[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0,
    0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03,
    0xC0, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00,
    0x03, 0xC0, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,
    0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x3F, 0xE3, 0xC7, 0xFC, 0x3F, 0xE3, 0xC7, 0xFC, 0x00, 0x03, 0xC0,
    0x00, 0x00, 0x03, 0xC0, 0x00, 0x3F, 0xC7, 0xE3, 0xFC, 0x3F, 0xC7, 0xF3, 0xFC, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x07,
    0xF0, 0x00, 0x1F, 0xC7, 0xE7, 0xF8, 0x1F, 0xE1, 0xC7, 0xF8, 0x00, 0x00, 0x00, 0x00};

const uint8_t back[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0xfc, 0x00,
    0x00, 0x01, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xe0, 0x7f, 0xff,
    0xff, 0xe0, 0x1f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xe0, 0x00,
    0x7c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};