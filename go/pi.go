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

//calculates the binary digits of pi up to 10000
func main() {
	fmt.Printf("3.");
	var n int;
	var s int;
	var x int;
	for x = 1; x<10000; x++ {
		s = 0;
		for n = 0; n < x; n++ {
			s += (((4/(8*n+1))-(2/(8*n+4))-(1/(8*n+5))-(1/(8*n+6)))*(1/(16^n)));
		}	
	}
}

