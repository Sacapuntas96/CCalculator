/* ccalculator-window.c
 *
 * Copyright 2026 Nassim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"

#include "ccalculator-window.h"

struct _CcalculatorWindow
{
	AdwApplicationWindow  parent_instance;

	/* Template widgets */
        GtkGrid *number_grid;
        GtkLabel *result_label;
        GtkLabel *history_label;
        CcalculatorWindow *main_window;

        int is_new;
        int is_decimal;
        int decimal;
        double first_digit;
        double second_digit;
        char operation;

};

G_DEFINE_FINAL_TYPE (CcalculatorWindow, ccalculator_window, ADW_TYPE_APPLICATION_WINDOW)

static void
ccalculator_window_class_init (CcalculatorWindowClass *klass)
{


	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/CCalculator/ccalculator-window.ui");
        gtk_widget_class_bind_template_child (widget_class, CcalculatorWindow, number_grid);
        gtk_widget_class_bind_template_child (widget_class, CcalculatorWindow, result_label);
        gtk_widget_class_bind_template_child (widget_class, CcalculatorWindow, history_label);
        GtkCssProvider *provider = gtk_css_provider_new ();

        gtk_css_provider_load_from_resource (provider, "/org/gnome/CCalculator/main.css");

        gtk_style_context_add_provider_for_display (
            gdk_display_get_default (),
            GTK_STYLE_PROVIDER (provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );

        g_object_unref (provider);
}

static void
operations (GtkButton *input, gpointer user_data, CcalculatorWindow *self){
        int i, len;
        char *result;
        const char* input_value = gtk_button_get_label (input);
        bool err;
        GtkLabel *result_label = self->result_label, *history_label = self->history_label;

        if(self->first_digit / 10 < 10e12){
          if(g_ascii_isdigit (input_value[0]) && input_value[0]){
            if(self->is_new){
              if(!self->is_decimal){
                  if(self->first_digit == 0){
                    self->first_digit = input_value[0] - '0';
                  }
                  else{
                    self->first_digit *= 10;
                    self->first_digit += input_value[0] - '0';
                  }
              }
              else{
                self->decimal *= 10;
                self->first_digit += (double) (input_value[0] - '0') / self->decimal;
              }
            }
            else{
              if(!self->is_decimal){
                  if(self->second_digit == 0){
                    self->second_digit = input_value[0] - '0';
                  }
                  else{
                    self->second_digit *= 10;
                    self->second_digit += input_value[0] - '0';
                  }
              }
              else{
                self->decimal *= 10;
                self->second_digit += ((double) (input_value[0] - '0') / self->decimal);
              }
            }
            if(self->is_new){
              result = g_strdup_printf("%f", self->first_digit);
            }
            else{
              result = g_strdup_printf("%f", self->second_digit);
            }
            len = strlen(result);
            i = len - 1;
            while(i >= 0){
              if(result[i] != '0'){
                if(result[i] == '.'){
                  result[i] = 0;
                }
                else{
                  result[i + 1] = 0;
                }
                break;
              }
              i--;
            }
            gtk_label_set_text (result_label, result);
        }
        else{
          if(strcmp(input_value, ".") == 0){
            self->is_decimal = 1;
          }
          else if(strcmp(input_value, "C") == 0){
            self->first_digit = self->second_digit = 0;
            self->is_decimal = 0;
            self->decimal = 1;
            self->is_new = 1;
            gtk_label_set_text(result_label, "0");
          }
          else if(strcmp(input_value, "=") == 0){
            if(self->is_new){
              gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit));
            }
            else{
              err = (self->second_digit == 0);
              switch(self->operation){
              case 'x': gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit * self->second_digit)); self->first_digit = self->first_digit * self->second_digit; break;
              case '/': if(!err)gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit / self->second_digit)); if(err)gtk_label_set_text(history_label, "Error"); if(!err)self->first_digit = self->first_digit / self->second_digit; break;
              case '+': gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit + self->second_digit)); self->first_digit = self->first_digit + self->second_digit; break;
              case '-': gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit - self->second_digit)); self->first_digit = self->first_digit - self->second_digit; break;
              default : gtk_label_set_text(history_label, "Error"); break;
              }
            }
            self->second_digit = 0;
            self->is_new = 1;
            self->is_decimal = 0;
            gtk_label_set_text(result_label, gtk_label_get_text(history_label));
            result = g_strdup_printf("%s", gtk_label_get_text(history_label));
        len = strlen(result);
        i = len - 1;

        if(atof (gtk_label_get_text(history_label)) != (int) atof(gtk_label_get_text(history_label)) || atof (gtk_label_get_text(history_label)) - (int) atof(gtk_label_get_text(history_label)) == 0){
          while(i >= 0){
            if(result[i] != '0'){
              if(result[i] == '.'){
                result[i] = 0;
              }
              else{
                result[i + 1] = 0;
              }
              break;
            }
            i--;
          }
          gtk_label_set_text (history_label, result);
        }

        result = g_strdup_printf("%s", gtk_label_get_text(result_label));
        len = strlen(result);
        i = len - 1;

        if(atof (gtk_label_get_text(result_label)) != (int) atof(gtk_label_get_text(result_label)) || atof (gtk_label_get_text(result_label)) - (int) atof(gtk_label_get_text(result_label)) == 0){
          while(i >= 0){
            if(result[i] != '0'){
              if(result[i] == '.'){
                result[i] = 0;
              }
              else{
                result[i + 1] = 0;
              }
              break;
            }
            i--;
          }
          gtk_label_set_text (result_label, result);
        }
          }
          else{
            if(self->is_new){
              self->is_new = 0;
              self->is_decimal = 0;
              self->decimal = 1;
              if(strcmp(input_value, "÷") == 0){
                self->operation = '/';
              }
              else if (strcmp(input_value, "×") == 0){
                self->operation = 'x';
              }
              else{
                self->operation = input_value[0];
              }
            }
            else{
              err = (self->second_digit == 0);
              switch(self->operation){
                case 'x': gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit * self->second_digit)); self->first_digit = self->first_digit * self->second_digit; break;
                case '/': if(!err)gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit / self->second_digit)); if(err)gtk_label_set_text(history_label, "Error"); if(!err)self->first_digit = self->first_digit / self->second_digit; break;
                case '+': gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit + self->second_digit)); self->first_digit = self->first_digit + self->second_digit; break;
                case '-': gtk_label_set_text(history_label, g_strdup_printf("%f", self->first_digit - self->second_digit)); self->first_digit = self->first_digit - self->second_digit; break;
                default : gtk_label_set_text(history_label, "Error"); break;
              }
              self->second_digit = 0;
              self->is_decimal = 0;
              self->decimal = 1;
              gtk_label_set_text(result_label, gtk_label_get_text(history_label));
              result = g_strdup_printf("%s", gtk_label_get_text(history_label));
        len = strlen(result);
        i = len - 1;

        if(atof (gtk_label_get_text(history_label)) != (int) atof(gtk_label_get_text(history_label)) || atof (gtk_label_get_text(history_label)) - (int) atof(gtk_label_get_text(history_label)) == 0){
          while(i >= 0){
            if(result[i] != '0'){
              if(result[i] == '.'){
                result[i] = 0;
              }
              else{
                result[i + 1] = 0;
              }
              break;
            }
            i--;
          }
          gtk_label_set_text (history_label, result);
        }

        result = g_strdup_printf("%s", gtk_label_get_text(result_label));
        len = strlen(result);
        i = len - 1;

        if(atof (gtk_label_get_text(result_label)) != (int) atof(gtk_label_get_text(result_label)) || atof (gtk_label_get_text(result_label)) - (int) atof(gtk_label_get_text(result_label)) == 0){
          while(i >= 0){
            if(result[i] != '0'){
              if(result[i] == '.'){
                result[i] = 0;
              }
              else{
                result[i + 1] = 0;
              }
              break;
            }
            i--;
          }
          gtk_label_set_text (result_label, result);
        }
            }
          }
        }
      }



}

static void
ccalculator_window_init (CcalculatorWindow *self){
        self->first_digit = self->second_digit = 0;
        self->is_new = 1;
        self->is_decimal = 0;
        self->decimal = 1;


	gtk_widget_init_template (GTK_WIDGET (self));


        for(GtkWidget *c_object = gtk_widget_get_first_child (GTK_WIDGET (self->number_grid)); c_object != NULL; c_object = gtk_widget_get_next_sibling (c_object)){
          g_signal_connect(c_object, "clicked", G_CALLBACK (operations), self);
          gtk_widget_set_focusable (c_object, FALSE);
        }

}


