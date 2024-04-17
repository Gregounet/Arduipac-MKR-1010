//
// ./arduipac_rom: traitement des fichiers odyssey2.bin(BIOS) et ufo.bin(CARTOUCHE)
//

#include <stdint.h>

uint8_t rom[4096] = {
	0x84, 0x00, 0x00, 0x84, 0x02, 0x00, 0x00, 0x84, 0x04, 0xC5, 0xAD, 0x09, 0xAE, 0x14, 0xE7, 0xB8, 
	0xA1, 0x80, 0x72, 0x18, 0xFE, 0x39, 0xFD, 0x93, 0x84, 0x06, 0xA5, 0xB5, 0xB9, 0xA2, 0xB8, 0x3D, 
	0x81, 0xA0, 0x18, 0x10, 0xF0, 0x53, 0x3F, 0xD3, 0x3C, 0x96, 0x2F, 0xF0, 0x53, 0xC0, 0xA0, 0x18, 
	0xF0, 0x37, 0xF2, 0x3A, 0x37, 0x53, 0x7F, 0xA0, 0x14, 0x89, 0xB9, 0x3F, 0xF1, 0x37, 0xD2, 0x14, 
	0xEB, 0x14, 0x84, 0x0A, 0xFC, 0xE3, 0xA9, 0x1C, 0xFC, 0xE3, 0xAA, 0xF9, 0xF2, 0x5C, 0xD2, 0x73, 
	0xB2, 0x7D, 0x92, 0x84, 0xB9, 0x3F, 0xF1, 0x53, 0xBF, 0xA1, 0x04, 0x14, 0x53, 0x7F, 0xAB, 0xB8, 
	0xAA, 0x27, 0x90, 0xB8, 0xA7, 0xB9, 0x03, 0xFA, 0xE3, 0x90, 0x18, 0x1A, 0xE9, 0x67, 0xFA, 0xE3, 
	0xAA, 0x04, 0x76, 0x53, 0x3F, 0xAB, 0x1C, 0xB8, 0xAA, 0xFA, 0x90, 0x04, 0x14, 0x53, 0x1F, 0xAB, 
	0xBA, 0x00, 0x04, 0x77, 0xFA, 0xAC, 0x84, 0x0A, 0x00, 0xB8, 0x7F, 0xB9, 0xA0, 0x81, 0x53, 0xD6, 
	0x91, 0x89, 0x7C, 0x99, 0xE7, 0x80, 0x96, 0xA3, 0x89, 0xBC, 0x99, 0xB7, 0xB9, 0xA0, 0x81, 0x43, 
	0x28, 0x91, 0x83, 0xAA, 0xC8, 0x80, 0xA9, 0xC8, 0x80, 0x91, 0xC8, 0x19, 0xEA, 0xA8, 0x04, 0x95, 
	0x15, 0xC5, 0x09, 0xAE, 0x89, 0xBC, 0x99, 0xBB, 0xB8, 0xF0, 0xBA, 0x06, 0xF8, 0x3A, 0x0A, 0x92, 
	0xDA, 0xBA, 0x30, 0xA8, 0x0A, 0xD8, 0x96, 0xDF, 0xEA, 0xC4, 0xF8, 0x47, 0x77, 0xD3, 0x07, 0x53, 
	0x3F, 0xA9, 0xFF, 0x53, 0x3F, 0xD9, 0x96, 0xE5, 0x04, 0xDF, 0x18, 0xEA, 0xBC, 0xBF, 0xFF, 0xFF, 
	0x43, 0xC0, 0xAF, 0x64, 0x78, 0x64, 0x76, 0x89, 0xBC, 0x99, 0xB7, 0x83, 0x89, 0xBC, 0x99, 0xAF, 
	0x83, 0x89, 0xBC, 0x99, 0xB7, 0x23, 0x20, 0xA8, 0xA9, 0x27, 0xA0, 0x18, 0xE9, 0xFA, 0x14, 0xEC, 
	0xB9, 0xFF, 0x91, 0xE9, 0x02, 0x91, 0x14, 0xE7, 0x34, 0x1C, 0xB8, 0xFF, 0xB9, 0x80, 0x27, 0x90, 
	0xC8, 0xE9, 0x0F, 0x23, 0xF8, 0x90, 0xE8, 0x15, 0x90, 0x34, 0x27, 0x83, 0x15, 0xC5, 0xB8, 0xA0, 
	0x80, 0x53, 0xD6, 0x90, 0xD5, 0x05, 0x83, 0x15, 0xC5, 0xB8, 0xA0, 0x80, 0x43, 0x28, 0x90, 0xD5, 
	0x05, 0x83, 0x27, 0x90, 0xB8, 0x3F, 0xF0, 0x43, 0x80, 0xA0, 0x14, 0xE7, 0x83, 0x34, 0x76, 0x14, 
	0xB0, 0xF2, 0x3D, 0x23, 0x56, 0x34, 0xA2, 0xC5, 0xFF, 0xD5, 0x83, 0xFE, 0x53, 0xFE, 0xAE, 0xFD, 
	0x97, 0xF7, 0xF7, 0xF7, 0xAD, 0x27, 0xF7, 0x4E, 0xAE, 0xFC, 0x67, 0x37, 0x17, 0x6D, 0xAD, 0xF6, 
	0x6A, 0xFE, 0x37, 0x53, 0x01, 0x2E, 0x53, 0xFE, 0x4E, 0xAE, 0x83, 0x23, 0xF8, 0xB8, 0x10, 0xBA, 
	0x30, 0x90, 0x18, 0xEA, 0x71, 0x83, 0xA5, 0x05, 0x76, 0x75, 0x24, 0x78, 0x14, 0xEC, 0xAA, 0xB8, 
	0x7F, 0x23, 0x08, 0x90, 0xC8, 0xF9, 0x90, 0xC8, 0xFA, 0x47, 0x53, 0x0F, 0xAD, 0x34, 0x97, 0xFA, 
	0x53, 0x0F, 0xAD, 0x34, 0x97, 0x24, 0x32, 0xFC, 0x90, 0xC8, 0xFB, 0x90, 0xC8, 0x03, 0x08, 0xAB, 
	0x44, 0x2C, 0x15, 0xC5, 0xAC, 0xBB, 0x01, 0xB8, 0x3F, 0xF0, 0x43, 0x40, 0xA0, 0xD5, 0x05, 0x83, 
	0xB8, 0x3E, 0xF0, 0xF2, 0x3A, 0x53, 0x3F, 0xD3, 0x3B, 0x96, 0x3A, 0x14, 0xEC, 0xF0, 0xB8, 0x02, 
	0xB9, 0x01, 0xD2, 0xE2, 0xBA, 0x99, 0x80, 0xC6, 0xCE, 0x6A, 0x57, 0x90, 0x24, 0xFC, 0x81, 0xC6, 
	0xDA, 0x23, 0x59, 0x90, 0x81, 0x6A, 0x57, 0x91, 0x24, 0xFC, 0xB8, 0x3E, 0xF0, 0x43, 0x80, 0xA0, 
	0x24, 0x3A, 0xBA, 0x01, 0x80, 0xD3, 0x59, 0xC6, 0xEF, 0x80, 0x6A, 0x57, 0x90, 0x24, 0xFC, 0x90, 
	0x81, 0xD3, 0x59, 0x96, 0xF8, 0x91, 0x24, 0xFC, 0x81, 0x6A, 0x57, 0x91, 0xB8, 0x7F, 0x54, 0x35, 
	0x23, 0x42, 0x90, 0xC8, 0xB9, 0x01, 0x81, 0x47, 0x54, 0x29, 0x54, 0x35, 0x23, 0x52, 0x90, 0xC8, 
	0x81, 0x54, 0x29, 0x54, 0x35, 0x23, 0x56, 0x90, 0xC8, 0x19, 0x81, 0x47, 0x54, 0x29, 0x54, 0x35, 
	0x23, 0x4A, 0x90, 0xC8, 0x81, 0x54, 0x29, 0x24, 0x32, 0x53, 0x0F, 0xAD, 0x34, 0x4B, 0xFD, 0x90, 
	0xC8, 0xFE, 0x90, 0xC8, 0x83, 0x23, 0x02, 0x90, 0xC8, 0x83, 0xB8, 0x40, 0xB9, 0x50, 0xFC, 0x90, 
	0x91, 0x18, 0x19, 0xFB, 0x90, 0x03, 0x08, 0x91, 0xBD, 0x0A, 0xB8, 0x46, 0x54, 0x61, 0xBD, 0x0C, 
	0xB8, 0x4E, 0x54, 0x61, 0xBD, 0x0C, 0xB8, 0x5A, 0x54, 0x61, 0xBD, 0x0C, 0xB8, 0x5E, 0x54, 0x61, 
	0x83, 0x34, 0x4B, 0xFD, 0x90, 0x18, 0xFE, 0x90, 0x18, 0x83, 0x07, 0xBA, 0xF8, 0x6A, 0xC9, 0xF6, 
	0x6D, 0x6A, 0x53, 0x07, 0x17, 0xAA, 0x27, 0x97, 0xA7, 0xF7, 0xEA, 0x79, 0xAA, 0x81, 0x5A, 0x83, 
	0x54, 0x6A, 0xC6, 0x89, 0x81, 0xDA, 0x91, 0x27, 0x17, 0x83, 0x54, 0x6A, 0x96, 0x92, 0x81, 0xDA, 
	0x91, 0x27, 0x83, 0x42, 0xAC, 0x53, 0x0F, 0xAD, 0x54, 0xA4, 0x2A, 0xAF, 0xFC, 0x53, 0xF0, 0x47, 
	0xAD, 0x54, 0xA4, 0x83, 0xFD, 0x03, 0xB3, 0xA3, 0x96, 0xAC, 0xAD, 0xCD, 0x1D, 0xAB, 0x6F, 0xF6, 
	0xA4, 0xFB, 0x83, 0x0F, 0x0E, 0x0D, 0x02, 0x0A, 0x05, 0x0B, 0x03, 0x06, 0x0C, 0x09, 0x01, 0x04, 
	0x08, 0x07, 0x00, 0xBF, 0xFF, 0xD5, 0x14, 0xF1, 0xB9, 0xF2, 0xB8, 0x10, 0xBA, 0x0B, 0xBB, 0x28, 
	0xBC, 0x70, 0xBE, 0x04, 0x34, 0x1C, 0xF9, 0xA3, 0xAD, 0x74, 0xEA, 0x19, 0x1E, 0x1E, 0xEA, 0xD6, 
	0x34, 0x27, 0x23, 0x4A, 0x34, 0xA2, 0x34, 0x3D, 0xA9, 0x34, 0x1C, 0x34, 0x6B, 0x34, 0x27, 0xF9, 
	0x84, 0x08, 0x19, 0x12, 0x0E, 0x12, 0x23, 0x14, 0x0C, 0x1C, 0x20, 0x26, 0x12, 0x20, 0x20, 0x20, 
	0x00, 0x0F, 0xFF, 0xCF, 0x03, 0xF0, 0x3F, 0xCF, 0x0F, 0x0F, 0x0F, 0xCF, 0x1C, 0x71, 0xC7, 0xCF, 
	0x33, 0x33, 0x33, 0xCF, 0x03, 0xF0, 0x3F, 0xEF, 0x0F, 0x0F, 0x0F, 0xEF, 0x1C, 0x71, 0xC7, 0xEF, 
	0x33, 0x33, 0x33, 0xEF, 0x55, 0x55, 0x55, 0xEF, 0x90, 0x10, 0x94, 0x00, 0x21, 0x00, 0x56, 0xDF, 
	0x53, 0xDD, 0x50, 0xDA, 0x4D, 0xD8, 0x4A, 0xD5, 0x47, 0xD2, 0x21, 0x00, 0x92, 0x20, 0x4A, 0xCD, 
	0x52, 0xEB, 0x4A, 0xC9, 0x52, 0xE7, 0x4A, 0xC5, 0x21, 0x00, 0x85, 0x04, 0x85, 0x0C, 0x85, 0x14, 
	0x85, 0x1C, 0x85, 0x24, 0x21, 0x00, 0x82, 0x1C, 0x21, 0x00, 0x8A, 0x00, 0x21, 0x00, 0x85, 0x24, 
	0x85, 0x1C, 0x85, 0x14, 0x85, 0x0C, 0x85, 0x04, 0x21, 0x00, 0x44, 0xFF, 0x4D, 0xDD, 0x48, 0xDA, 
	0x44, 0xD5, 0x42, 0xD3, 0x21, 0x00, 0xF9, 0xAF, 0xFE, 0x39, 0xFF, 0xD5, 0x05, 0x83, 0x20, 0x99, 
	0xFE, 0x84, 0x08, 0x99, 0xFD, 0x84, 0x08, 0x99, 0xFC, 0x84, 0x08, 0x89, 0x03, 0x84, 0x08, 0x09, 
	0xA8, 0x89, 0xBC, 0x99, 0x9B, 0xF9, 0x43, 0xF0, 0x3A, 0x85, 0x27, 0xAA, 0xAB, 0x08, 0xA9, 0x12, 
	0xA2, 0xCB, 0x32, 0xA5, 0x1A, 0x52, 0xA8, 0x1B, 0x72, 0xAB, 0xCA, 0x92, 0xAE, 0x95, 0xF8, 0x39, 
	0x83, 0xFB, 0x53, 0x0F, 0xA8, 0xFA, 0x53, 0x0F, 0x47, 0x48, 0xA8, 0xB9, 0x07, 0x23, 0xC7, 0x69, 
	0xA3, 0xD8, 0xC6, 0xC6, 0xE9, 0xBD, 0x83, 0x10, 0x1F, 0x0F, 0xFF, 0xF0, 0xF1, 0x01, 0x11, 0xBC, 
	0xFF, 0xFB, 0x37, 0x17, 0xAE, 0xFA, 0x6E, 0x1C, 0xF6, 0xD6, 0x6B, 0xAD, 0x83, 0xFA, 0x96, 0xE1, 
	0x83, 0xFB, 0x96, 0xE5, 0x83, 0x27, 0x6B, 0xEA, 0xE6, 0x83, 0xFC, 0x90, 0x18, 0xF8, 0xD2, 0xF9, 
	0xFB, 0x90, 0x03, 0x08, 0xAB, 0x18, 0x54, 0x61, 0x83, 0x53, 0x0F, 0x07, 0xC6, 0xF0, 0x64, 0xF5, 
	0x44, 0xC3, 0x04, 0x09, 0x93, 0x00, 0xC4, 0x00, 0xA4, 0x00, 0xFC, 0xF2, 0x0F, 0x04, 0x44, 0xE4, 
	0x00, 0xB8, 0x25, 0x10, 0xF5, 0x34, 0x76, 0x14, 0x2B, 0x54, 0x00, 0xE5, 0x94, 0xB0, 0x94, 0x24, 
	0xA4, 0x00, 0xA4, 0x7E, 0xBF, 0x01, 0xB9, 0x7A, 0x81, 0x12, 0x22, 0x19, 0x94, 0x3E, 0xBF, 0x02, 
	0xB9, 0x77, 0x94, 0x3E, 0xBF, 0x03, 0xB9, 0x73, 0x94, 0x3E, 0xBF, 0x04, 0xB9, 0x6F, 0x81, 0xC6, 
	0x70, 0xD3, 0x18, 0x53, 0x38, 0xC6, 0x70, 0x19, 0x19, 0xB8, 0x3D, 0xF0, 0x67, 0xEF, 0x4C, 0xE6, 
	0x70, 0xBE, 0x0C, 0xB8, 0x6D, 0x94, 0x5D, 0xC8, 0xC8, 0xC8, 0xEE, 0x55, 0x83, 0xF5, 0x74, 0x00, 
	0xE5, 0x03, 0xF3, 0xC8, 0xF6, 0x70, 0xC9, 0xF5, 0x74, 0x00, 0xE5, 0x03, 0xF8, 0xE6, 0x71, 0x19, 
	0x83, 0xF9, 0xD3, 0x7C, 0x96, 0x80, 0xB9, 0x2B, 0xF1, 0x92, 0x80, 0xB9, 0x7A, 0x23, 0x01, 0x91, 
	0xBC, 0x38, 0xB4, 0x9D, 0x23, 0xF8, 0x90, 0xC8, 0x90, 0xC8, 0xC8, 0x80, 0xBC, 0x00, 0xD2, 0x9A, 
	0xBC, 0x0A, 0xB2, 0x9A, 0xBC, 0x03, 0x92, 0x9A, 0xBC, 0x01, 0xF5, 0x14, 0x10, 0xE5, 0xBE, 0x01, 
	0x00, 0xB8, 0x13, 0x80, 0x6C, 0x90, 0x03, 0xF6, 0xE6, 0x70, 0x90, 0xBC, 0x01, 0xC8, 0x84, 0xA3, 
	0xB8, 0x22, 0x80, 0xC6, 0xB8, 0x07, 0x90, 0x83, 0xB9, 0x00, 0x74, 0x8F, 0xFA, 0x4B, 0xC6, 0xFD, 
	0x74, 0xB1, 0xF9, 0xC6, 0xCA, 0x37, 0x17, 0x03, 0x08, 0xE7, 0xAC, 0xB8, 0x23, 0xBF, 0x00, 0x80, 
	0xDC, 0xC6, 0xDE, 0xCC, 0x1F, 0xFC, 0x17, 0x96, 0xDC, 0x23, 0x0F, 0xAC, 0x84, 0xCF, 0xFF, 0xC6, 
	0xFB, 0x03, 0xFB, 0xBC, 0x04, 0xF6, 0xE9, 0xBC, 0x10, 0x80, 0x17, 0x90, 0xD3, 0x10, 0x96, 0xF1, 
	0x90, 0xC5, 0xFB, 0xD5, 0x96, 0xFA, 0xB9, 0x27, 0xB1, 0xC4, 0xFC, 0xC8, 0x90, 0x83, 0x00, 0x00, 
	0x14, 0xEC, 0xF4, 0x92, 0xB4, 0x3E, 0xD4, 0x63, 0xB9, 0x2E, 0xF1, 0x96, 0x12, 0x11, 0xF4, 0x5D, 
	0xF4, 0x4D, 0xB8, 0x27, 0xF0, 0xC6, 0x1B, 0xB0, 0x00, 0x34, 0xA2, 0xBA, 0x27, 0xBB, 0xAA, 0x15, 
	0xF4, 0x16, 0xF4, 0x08, 0xD4, 0xAB, 0x34, 0x76, 0x34, 0x34, 0xB8, 0xA3, 0x23, 0x00, 0x90, 0xC8, 
	0x23, 0x8F, 0x90, 0x14, 0xEC, 0xD4, 0x03, 0xB8, 0x08, 0x23, 0x36, 0x90, 0x84, 0x11, 0xB8, 0x7A, 
	0x80, 0x12, 0x44, 0x83, 0xB9, 0x7C, 0x81, 0xD3, 0xF8, 0x96, 0x43, 0x80, 0x32, 0x50, 0xC4, 0x30, 
	0xB9, 0x27, 0xB1, 0x00, 0xC9, 0xBC, 0x80, 0xB8, 0x2D, 0x10, 0xF0, 0x53, 0x03, 0x96, 0x43, 0xA0, 
	0x11, 0xBF, 0x06, 0xB8, 0x09, 0x80, 0xF2, 0x70, 0x18, 0xEF, 0x65, 0xC8, 0x80, 0x4C, 0x90, 0x83, 
	0xDC, 0x90, 0xF1, 0xD2, 0x7A, 0x19, 0xB1, 0x4A, 0xA4, 0x6B, 0x27, 0xA1, 0xE4, 0x5D, 0x03, 0x04, 
	0x91, 0x53, 0x38, 0x96, 0x9C, 0x19, 0x81, 0x03, 0x08, 0x91, 0xAC, 0xF2, 0x96, 0xB8, 0x7C, 0xB4, 
	0x9D, 0xF5, 0x14, 0x10, 0xE5, 0x83, 0x19, 0x23, 0xF8, 0x91, 0x19, 0x91, 0x83, 0xB9, 0x79, 0x18, 
	0xBF, 0x03, 0x80, 0x91, 0xC8, 0xC9, 0x80, 0x91, 0x18, 0xC9, 0xFC, 0x91, 0xC9, 0xC9, 0xEF, 0xA2, 
	0x83, 0xBE, 0x3C, 0x81, 0xC6, 0xC2, 0xD3, 0xF8, 0xC6, 0xC2, 0xCE, 0xC9, 0xC9, 0xC9, 0xC9, 0xEF, 
	0xB3, 0x83, 0xB8, 0x25, 0xF0, 0x12, 0xDC, 0xBA, 0x01, 0xBD, 0x0C, 0x32, 0xD1, 0xBD, 0x04, 0xBA, 
	0xA0, 0x42, 0x53, 0x7F, 0x03, 0x1B, 0x91, 0xC9, 0xFA, 0x91, 0xA4, 0xEF, 0xBA, 0x0B, 0xBD, 0x00, 
	0x32, 0xE6, 0xBD, 0x08, 0xBA, 0xB8, 0xFA, 0x91, 0xC9, 0x42, 0x53, 0x7F, 0x03, 0x10, 0x91, 0xC9, 
	0xC9, 0x42, 0x53, 0x0F, 0x91, 0xFE, 0xA8, 0x42, 0x47, 0x62, 0x53, 0x07, 0x6D, 0xA0, 0x83, 0x00, 
	0x55, 0x04, 0x1A, 0x14, 0xB0, 0x15, 0xAA, 0xB9, 0x24, 0xF2, 0x19, 0xF1, 0xF2, 0x18, 0x17, 0x43, 
	0x80, 0xA1, 0xD3, 0x87, 0x96, 0x18, 0xB1, 0x86, 0x83, 0xB8, 0x08, 0xF1, 0x53, 0x07, 0xA1, 0x68, 
	0xA8, 0xFA, 0x53, 0x3F, 0xAA, 0x03, 0xD2, 0xF6, 0x18, 0xFA, 0x90, 0xB8, 0x27, 0xB0, 0x56, 0x83, 
	0x23, 0x03, 0x90, 0xB8, 0x04, 0xB9, 0x10, 0xBF, 0x04, 0x81, 0x17, 0x37, 0x17, 0xAA, 0x80, 0x17, 
	0x6A, 0xC6, 0x4A, 0x85, 0x95, 0xF6, 0x54, 0x85, 0xE6, 0x4E, 0x18, 0x19, 0xEF, 0x39, 0xB8, 0x24, 
	0xB0, 0x80, 0xF4, 0x4D, 0xB8, 0x13, 0xB9, 0x07, 0xBF, 0x04, 0x80, 0xB6, 0x5E, 0x91, 0xC9, 0xC8, 
	0xEF, 0x5A, 0x83, 0xBA, 0x0C, 0xB8, 0x6D, 0x80, 0xAC, 0xC8, 0xC8, 0xC8, 0x80, 0xBD, 0x3C, 0xB2, 
	0x83, 0xD2, 0x96, 0xB9, 0x25, 0xF1, 0xBD, 0x31, 0x12, 0x81, 0x80, 0xCC, 0xCC, 0x92, 0x83, 0x1C, 
	0x1C, 0xF4, 0x26, 0x80, 0xAE, 0x34, 0x4B, 0x18, 0xFD, 0x90, 0xC8, 0x80, 0x53, 0xF0, 0x4E, 0x90, 
	0xB2, 0xA3, 0xC8, 0xEA, 0x67, 0x83, 0xB9, 0x2F, 0xF1, 0x19, 0xD1, 0xBD, 0x2E, 0x72, 0xA1, 0xBD, 
	0x3B, 0xC4, 0x83, 0x18, 0x80, 0x03, 0x05, 0x90, 0xC8, 0xC4, 0x92, 0xB8, 0x2C, 0xF0, 0x10, 0xA5, 
	0x12, 0xB3, 0xB5, 0x15, 0x89, 0x7C, 0x99, 0xE7, 0xF4, 0x30, 0xBC, 0xC7, 0xB8, 0x0C, 0xB9, 0x62, 
	0x76, 0xC6, 0xB8, 0x04, 0xB9, 0x42, 0xBB, 0x04, 0xD4, 0xDC, 0xB9, 0x72, 0xB8, 0x0D, 0x76, 0xD4, 
	0xB9, 0x52, 0xB8, 0x05, 0xBB, 0x04, 0xD4, 0xDC, 0x14, 0xE7, 0xE4, 0x12, 0x80, 0xBD, 0x0C, 0xF2, 
	0xE2, 0xAD, 0xF8, 0xBE, 0x02, 0x92, 0xEF, 0xBE, 0x04, 0xD3, 0x08, 0x96, 0xEF, 0xBE, 0x0E, 0x18, 
	0x18, 0x34, 0x4B, 0xFD, 0x91, 0x19, 0xFE, 0x91, 0x19, 0x19, 0x19, 0xEB, 0xDC, 0x83, 0x00, 0x00, 
	0xA3, 0xA9, 0x1C, 0xFC, 0xA3, 0xAA, 0x04, 0x4B, 0x14, 0xE7, 0x23, 0x80, 0xC5, 0xB8, 0xA0, 0x90, 
	0xD5, 0x83, 0x23, 0xA8, 0xE4, 0x0C, 0x14, 0xE7, 0xB8, 0xA0, 0xB9, 0xA5, 0xFB, 0x90, 0x81, 0xC9, 
	0x81, 0x97, 0x6A, 0xE6, 0x1A, 0x83, 0xB9, 0x25, 0xF1, 0xBD, 0x29, 0x52, 0x2F, 0xBD, 0x10, 0x83, 
	0xB8, 0x41, 0xBB, 0xC7, 0x23, 0x11, 0xF4, 0x48, 0xB8, 0x51, 0x23, 0x19, 0xF4, 0x48, 0xB8, 0x61, 
	0x23, 0x52, 0xF4, 0x48, 0xB8, 0x71, 0x23, 0x5A, 0x90, 0xC8, 0xFB, 0x90, 0x83, 0xBF, 0x08, 0xB8, 
	0x55, 0xB9, 0x0F, 0xE4, 0x65, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x36, 0xF4, 0x6D, 0xBF, 
	0x06, 0xB8, 0x8C, 0xB9, 0x7F, 0xF8, 0xA3, 0x91, 0x18, 0xC9, 0xEF, 0x65, 0x83, 0xBF, 0x0F, 0xB9, 
	0x79, 0x23, 0xF8, 0x91, 0xC9, 0x91, 0xC9, 0x27, 0x91, 0xC9, 0x91, 0xC9, 0xEF, 0x71, 0xBF, 0x04, 
	0xB9, 0x10, 0x91, 0x19, 0xEF, 0x82, 0xB9, 0x28, 0xA1, 0x19, 0xA1, 0x83, 0x40, 0x00, 0x60, 0x51, 
	0x08, 0x00, 0xB9, 0x2A, 0x11, 0xF1, 0x53, 0x03, 0x96, 0xA3, 0x19, 0x11, 0xF1, 0x03, 0xF0, 0xE6, 
	0xF0, 0xB1, 0x10, 0x83, 0x42, 0xFF, 0x81, 0x20, 0x81, 0x1C, 0x21, 0x00, 0x41, 0xFF, 0x82, 0x1C, 
	0x42, 0xFF, 0x82, 0x08, 0x10, 0x2E, 0x42, 0xDF, 0x82, 0x04, 0x10, 0x32, 0x84, 0x10, 0x21, 0x00, 
	0x44, 0xFF, 0x21, 0x00, 0x81, 0x24, 0x81, 0x14, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xB9, 0x27, 0xB1, 0xF5, 0x83, 0x81, 0x1C, 0x44, 0xC2, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xBC, 0xD1, 0xAF, 0x1F, 0x1C, 0x1C, 0xEF, 0x04, 0xFC, 0xA3, 0xAB, 0x1C, 0xFC, 0xA3, 0xAA, 0x83, 
	0xB8, 0x15, 0xBF, 0x09, 0xBA, 0x22, 0xFA, 0xA3, 0x90, 0x1A, 0x18, 0xEF, 0x16, 0xB8, 0x27, 0xB0, 
	0xAC, 0x83, 0xA8, 0xFF, 0x01, 0xA8, 0xFF, 0xFF, 0xA8, 0x01, 0x00, 0xB8, 0x25, 0xF0, 0xA5, 0x12, 
	0x32, 0xB5, 0xB8, 0x14, 0x80, 0x17, 0x90, 0xB9, 0x79, 0xB8, 0x15, 0xBA, 0x00, 0x14, 0x78, 0xB9, 
	0x75, 0xB8, 0x18, 0xBA, 0x05, 0x14, 0x78, 0xB9, 0x71, 0xB8, 0x1B, 0xBA, 0x0A, 0x14, 0x78, 0xB8, 
	0x3D, 0x23, 0x10, 0x90, 0xC8, 0x23, 0x80, 0xB9, 0x15, 0x76, 0x5F, 0xB9, 0x1B, 0x23, 0x90, 0x90, 
	0x81, 0xB9, 0x33, 0x14, 0x6C, 0x76, 0xA0, 0xB9, 0x18, 0x81, 0xB9, 0x3B, 0xBC, 0xE8, 0xD2, 0x76, 
	0xBC, 0xEF, 0xB2, 0x76, 0xBC, 0x6E, 0x24, 0xD6, 0x80, 0xB2, 0xA1, 0xD2, 0xA1, 0xF2, 0x85, 0x96, 
	0x83, 0xB6, 0xB7, 0x24, 0x00, 0x53, 0x7F, 0xC6, 0xAB, 0x80, 0x07, 0x90, 0x18, 0x80, 0xAB, 0x18, 
	0x80, 0xAA, 0x81, 0x6B, 0x91, 0xC9, 0x81, 0x6A, 0x91, 0xB9, 0x2B, 0xB1, 0x00, 0xC9, 0xB1, 0x00, 
	0x83, 0x07, 0x90, 0x53, 0x1F, 0xC6, 0xAB, 0x80, 0xB2, 0x8A, 0x83, 0x90, 0xC9, 0xC9, 0x91, 0xB9, 
	0x23, 0x81, 0x17, 0xB9, 0x14, 0x91, 0x83, 0xC9, 0xC9, 0x81, 0x19, 0x19, 0xD3, 0x38, 0x96, 0x83, 
	0x23, 0x90, 0x90, 0x18, 0xB9, 0x23, 0x81, 0x14, 0x00, 0xFB, 0x90, 0x18, 0xFA, 0x90, 0xB9, 0x27, 
	0xB1, 0xA4, 0x83, 0x00, 0x03, 0x01, 0x03, 0x02, 0x02, 0x03, 0x01, 0x04, 0x00, 0x03, 0xFF, 0x02, 
	0xFE, 0x01, 0xFD, 0x00, 0xFD, 0xFF, 0xFD, 0xFE, 0xFE, 0xFD, 0xFF, 0xFC, 0x00, 0xFD, 0x01, 0xFE, 
	0x02, 0xFF, 0x03, 0xB8, 0x28, 0xF0, 0xC6, 0xFA, 0x10, 0x83, 0xBD, 0x1C, 0x54, 0x09, 0x24, 0xF7, 
	0xB8, 0x14, 0x80, 0x03, 0xF0, 0xE6, 0x09, 0x27, 0x90, 0x80, 0x6A, 0xAF, 0x03, 0xF0, 0xE6, 0x11, 
	0xAF, 0x1F, 0xBC, 0x4C, 0xB8, 0x2B, 0xF0, 0x17, 0x37, 0x17, 0x6F, 0xBE, 0x00, 0xF6, 0x21, 0xBE, 
	0x20, 0xB8, 0x23, 0x80, 0x17, 0x37, 0x17, 0x6F, 0xF6, 0x2C, 0x37, 0x17, 0x03, 0xFE, 0xF6, 0x35, 
	0x80, 0xAF, 0x1F, 0xBE, 0x38, 0x1C, 0x1C, 0xEF, 0x35, 0xFC, 0xA3, 0xAB, 0x1C, 0xFC, 0xA3, 0xAA, 
	0xB8, 0x7D, 0x80, 0x6B, 0x91, 0xC8, 0xC9, 0x80, 0x6A, 0x91, 0xC9, 0xFE, 0x91, 0x83, 0x00, 0x09, 
	0x03, 0x08, 0x05, 0x06, 0x07, 0x04, 0x08, 0x01, 0x07, 0xFE, 0x05, 0xFB, 0x03, 0xF9, 0x00, 0xF8, 
	0xFD, 0xF9, 0xFA, 0xFB, 0xF8, 0xFE, 0xF7, 0x01, 0xF8, 0x04, 0xFA, 0x06, 0xFC, 0x08, 0x00, 0x00, 
	0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0xB9, 0x00, 0xE5, 0x74, 0x8F, 0xF5, 0xB8, 0x2B, 0xF0, 0x92, 
	0x86, 0xB8, 0x25, 0xF0, 0x12, 0xBB, 0xB8, 0x7D, 0x80, 0xD3, 0xF8, 0xC6, 0xDF, 0x80, 0x6B, 0x90, 
	0xBC, 0xF0, 0x6C, 0xF6, 0x97, 0x34, 0xCF, 0xBC, 0x48, 0x80, 0x6C, 0xE6, 0x9F, 0x34, 0xCF, 0xC8, 
	0x80, 0x6A, 0x90, 0xBC, 0xF8, 0x6C, 0xF6, 0xAA, 0x34, 0xCF, 0x80, 0xBC, 0x70, 0x6C, 0xE6, 0xB2, 
	0x34, 0xCF, 0xC8, 0xC8, 0x80, 0x18, 0x12, 0xBB, 0x23, 0x09, 0x90, 0xBC, 0xE0, 0x34, 0xD4, 0xB8, 
	0x25, 0xF0, 0x77, 0x77, 0x53, 0x07, 0xAF, 0xB9, 0x37, 0x81, 0xE7, 0xEF, 0xCA, 0x91, 0x83, 0xFC, 
	0x37, 0x17, 0x90, 0x83, 0xB9, 0x3B, 0xBF, 0x08, 0xFC, 0xA3, 0x91, 0x1C, 0xC9, 0xEF, 0xD8, 0x83, 
	0x00, 0x00, 0x18, 0x3C, 0xEF, 0x3C, 0x00, 0x00, 0x09, 0xD2, 0x26, 0x7C, 0x3F, 0x24, 0x54, 0x82, 
	0x11, 0x44, 0x20, 0x88, 0x02, 0x20, 0x89, 0xBF, 0x0A, 0xB9, 0x6D, 0xE5, 0xB4, 0xB1, 0xF5, 0x83, 
	0x14, 0xF3, 0x74, 0xA6, 0x54, 0x20, 0x54, 0x5F, 0x83, 0xB9, 0x10, 0xBF, 0x03, 0x81, 0x96, 0x13, 
	0x19, 0xEF, 0x0D, 0xFD, 0x6F, 0xA3, 0xA0, 0x83, 0xFF, 0x7F, 0x3F, 0x1F, 0xC0, 0xE0, 0xF0, 0x00, 
	0xB9, 0x25, 0xF1, 0x03, 0x03, 0x53, 0x0F, 0xAF, 0x03, 0xF5, 0xE6, 0x2F, 0x03, 0x04, 0xAF, 0x1F, 
	0xB9, 0x6E, 0xC9, 0xC9, 0xC9, 0xC9, 0xEF, 0x32, 0x81, 0x92, 0x5B, 0x83, 0xB8, 0x7C, 0x19, 0x19, 
	0x27, 0xAB, 0xAA, 0x74, 0x00, 0xAD, 0xC6, 0x4E, 0xBA, 0xFE, 0xF6, 0x4E, 0xBA, 0x02, 0x19, 0x18, 
	0x74, 0x00, 0xC6, 0x5A, 0xBB, 0xFE, 0xF6, 0x5A, 0xBB, 0x02, 0x83, 0x54, 0x3C, 0x64, 0xD7, 0xB8, 
	0x25, 0xF0, 0x53, 0x03, 0x96, 0xA1, 0xB9, 0x45, 0xBE, 0x32, 0x81, 0xC6, 0x71, 0xD3, 0xF8, 0x96, 
	0x97, 0xB8, 0x29, 0x10, 0xF0, 0x96, 0x08, 0xBD, 0x18, 0x54, 0x09, 0xB9, 0x45, 0xAC, 0x42, 0x5C, 
	0xA0, 0xE5, 0xB4, 0xC5, 0xF5, 0xB9, 0x42, 0x81, 0x43, 0x20, 0x91, 0x54, 0x3C, 0xB8, 0x22, 0xFB, 
	0xC6, 0x93, 0x67, 0xA0, 0x18, 0xFA, 0xA0, 0xB9, 0x45, 0xB8, 0x27, 0xB0, 0xBC, 0xB8, 0x23, 0x44, 
	0xEA, 0xB9, 0x41, 0x81, 0xD3, 0xF8, 0x96, 0xE6, 0x42, 0x12, 0xFD, 0xB9, 0x45, 0x81, 0xD3, 0xF8, 
	0xC6, 0xFD, 0xB9, 0x42, 0x54, 0x3C, 0xC6, 0xFD, 0x97, 0x67, 0xAC, 0xFD, 0xC6, 0xFD, 0x74, 0x03, 
	0x03, 0xF0, 0xF6, 0xFD, 0xB8, 0x30, 0xFA, 0xA0, 0xC8, 0xFB, 0xA0, 0xB8, 0x41, 0x81, 0xBF, 0x04, 
	0x6B, 0xEF, 0xD0, 0x90, 0xC8, 0xC9, 0xBF, 0x04, 0x81, 0x6A, 0xEF, 0xD9, 0x90, 0xC8, 0xC8, 0x23, 
	0x4E, 0x90, 0xB9, 0x27, 0xB1, 0xC0, 0xB9, 0x41, 0xB8, 0x30, 0x81, 0xD3, 0xF8, 0xC6, 0xFA, 0xF0, 
	0xAA, 0xC8, 0xF0, 0x97, 0xF7, 0xAB, 0xB8, 0x32, 0x64, 0xD7, 0xA0, 0xC8, 0xA0, 0x83, 0x00, 0x00, 
	0x81, 0xAC, 0x80, 0x37, 0x17, 0x6C, 0xF6, 0x0A, 0x37, 0x17, 0xAC, 0x83, 0xF8, 0xAF, 0xB8, 0x6D, 
	0xBE, 0x0C, 0xF8, 0xD9, 0xC6, 0x1D, 0x80, 0xD3, 0xF8, 0xC6, 0x1D, 0x74, 0x24, 0xC8, 0xC8, 0xC8, 
	0xC8, 0xEE, 0x12, 0x83, 0x81, 0x6B, 0x74, 0x01, 0xAD, 0x03, 0xEC, 0x85, 0xF6, 0x68, 0x95, 0xC8, 
	0xC9, 0x81, 0x6A, 0x74, 0x01, 0x03, 0xF3, 0xC6, 0x45, 0xF6, 0x66, 0x23, 0xFA, 0x6C, 0xF6, 0x45, 
	0x23, 0xF3, 0x6D, 0xE6, 0x69, 0xC9, 0xC9, 0x81, 0x53, 0x70, 0x96, 0x5F, 0x95, 0xB6, 0x57, 0x97, 
	0xA7, 0xFA, 0xF2, 0x55, 0x97, 0x67, 0xAA, 0x97, 0xA7, 0xFB, 0xF2, 0x5D, 0x97, 0x67, 0xAB, 0x19, 
	0x19, 0xFF, 0x28, 0x10, 0x28, 0xAF, 0x18, 0x19, 0x83, 0xC8, 0xC8, 0x80, 0x43, 0x10, 0x90, 0xB8, 
	0x15, 0xBC, 0x78, 0xBE, 0x02, 0x80, 0xF2, 0x7C, 0xD2, 0x7C, 0x64, 0x85, 0xCC, 0xCC, 0xCC, 0xCC, 
	0x18, 0x18, 0x18, 0xEE, 0x75, 0x23, 0xCF, 0x90, 0xFC, 0xA8, 0x81, 0x90, 0x18, 0x19, 0x81, 0x90, 
	0xC8, 0xC8, 0x23, 0x18, 0x90, 0x23, 0xF8, 0x91, 0xC9, 0x91, 0xBE, 0x01, 0xC5, 0xFB, 0xD5, 0x96, 
	0xA5, 0xB9, 0x27, 0xB1, 0xB6, 0x83, 0xB8, 0x25, 0xF0, 0x53, 0x0F, 0xAF, 0x03, 0xF5, 0xE6, 0xB3, 
	0x03, 0x04, 0xAF, 0x1F, 0xB8, 0x3D, 0xB9, 0x6E, 0xC8, 0xC9, 0xC9, 0xC9, 0xC9, 0xEF, 0xB8, 0x81, 
	0x92, 0xF2, 0x19, 0x19, 0x19, 0x81, 0xD3, 0xF8, 0xC6, 0xF2, 0xF0, 0x03, 0x10, 0xA0, 0x53, 0xF0, 
	0xC6, 0xF3, 0xF0, 0x53, 0x0F, 0x14, 0x00, 0x74, 0x0C, 0x81, 0x6B, 0x91, 0xC6, 0xED, 0x03, 0x47, 
	0xF6, 0xED, 0xC9, 0x81, 0x6A, 0x91, 0x19, 0xC6, 0xED, 0x03, 0x50, 0xE6, 0xF2, 0x23, 0xF8, 0x91, 
	0xC9, 0x91, 0x83, 0x10, 0xF0, 0xD3, 0x10, 0x96, 0xFA, 0xA0, 0xF0, 0x43, 0x10, 0xA0, 0x64, 0xD2, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// ./arduipac_rom: fin des fichiers odyssey2.bin et ufo.bin
