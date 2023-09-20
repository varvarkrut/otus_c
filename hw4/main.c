#include <stdio.h>
#include <curl/curl.h>
#include <jansson.h>
#include "stdlib.h"
#include "string.h"


struct memory {
    char *response;
    size_t size;
};

static size_t cb(void *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)clientp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL)
        return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

int parse_json_and_print_the_weater(char* response){
    json_t *json = NULL;
    json_error_t error;
    json = json_loads(response, 0, &error);
    if (!json) {
        printf("Ошибка чтения JSON: %s\n", error.text);
        return 1;
    }
    json_t *current_condition_array = json_object_get(json, "current_condition");
    if (current_condition_array && json_is_array(current_condition_array)) {
        // Получаем первый объект в массиве current_condition
        json_t *current_condition_obj = json_array_get(current_condition_array, 0);
        if (current_condition_obj && json_is_object(current_condition_obj)) {
            const char *feels_like_c = json_string_value(json_object_get(current_condition_obj, "FeelsLikeC"));
            const char *feels_like_f = json_string_value(json_object_get(current_condition_obj, "FeelsLikeF"));
            const char *cloudcover = json_string_value(json_object_get(current_condition_obj, "cloudcover"));
            const char *humidity = json_string_value(json_object_get(current_condition_obj, "humidity"));

            printf("FeelsLikeC: %s\n", feels_like_c);
            printf("FeelsLikeF: %s\n", feels_like_f);
            printf("Cloudcover: %s\n", cloudcover);
            printf("Humidity: %s\n", humidity);

            // Получаем массив lang_ru
            json_t *lang_ru_array = json_object_get(current_condition_obj, "lang_ru");
            if (lang_ru_array && json_is_array(lang_ru_array)) {
                // Получаем первый объект в массиве lang_ru
                json_t *lang_ru_obj = json_array_get(lang_ru_array, 0);
                if (lang_ru_obj && json_is_object(lang_ru_obj)) {
                    const char *value_ru = json_string_value(json_object_get(lang_ru_obj, "value"));
                    printf("Lang_ru value: %s\n", value_ru);
                }
            }
        }
    }
    return 0;
}

int get_weather(const char* city) {
    char url[100]; // Создаем массив символов для хранения URL

    CURL *curl;
    CURLcode res;
    struct memory chunk = {0};
    sprintf(url, "https://wttr.in/%s?format=j1", city);
    curl = curl_easy_init();
    if (curl) {
        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));


        // Освобождаем ресурсы cURL
        curl_easy_cleanup(curl);
        parse_json_and_print_the_weater(chunk.response);
        return 0;
    } else{
        return 1;
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <city> \n", argv[0]);
        return 1;
    }
    char* city = argv[1];
    get_weather(city);
    return 0;
}
