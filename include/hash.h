#pragma once

/**
 * @brief djb2 algorithm by dan bernstein
 * @file hash.h
 * 
 * @param str data to map
 * @return unsigned long hash value
 */
unsigned long hash(char *str);