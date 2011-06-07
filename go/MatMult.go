/*
 *	Copyright (C) 2011  Kiel Friedt
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package main

import "fmt"

//Matrix multiplier of a 1000x1000 matrix.
func main() {
	var a [1000][1000]int
	var b [1000][1000]int
	var c [1000][1000]int
	var i int
	var j int
	for i = 0; i < 1000; i++ {
		for j = 0; j < 1000; j++ {
			a[i][j] = i + j
			b[i][j] = i + j
		}
	}
	for i = 0; i < 1000; i++ {
		for j = 0; j < 1000; j++ {
			c[i][j] += a[i][j] * b[j][i]
		}
	}
	for i = 0; i < 1000; i++ {
		for j = 0; j < 1000; j++ {
			fmt.Printf(" %d ", c[i][j])
		}
		fmt.Printf("\n");
	}
}

