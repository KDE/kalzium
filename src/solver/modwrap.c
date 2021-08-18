/*
    SPDX-FileCopyrightText: 2004 Thomas Nagy
    tnagy2^8@yahoo.fr

    SPDX-License-Identifier: GPL-2.0-or-later

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include <stdio.h>
#include <string.h>

#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/alloc.h>

char* solve_equation(const char *eq)
{
    static value *solve_equation_closure = NULL;
    if (solve_equation_closure == NULL) {
        solve_equation_closure = caml_named_value("solve_equation");
    }

    value v = copy_string(eq);
    return strdup(String_val(callback(*solve_equation_closure, v)));
}
