#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ASCII_CHARS 95

struct node
{
    char code[2];
    char c;
    int frequency;
    struct node *left;
    struct node *right;
    struct node *parent;
};

struct node *construct_huffman_tree(struct node *frequency_table[], int total_frequency, struct node *leaf_nodes[])
{
    struct node *root;
    int min1 = INT_MAX, min2 = INT_MAX;
    int min1value = 0, min2value = 0;
    int i = 0;
    while (1)
    {
        min1value = INT_MAX;
        min2value = INT_MAX;
        for (i = 0; i < ASCII_CHARS; i++)
        {
            if (frequency_table[i] == NULL)
                continue;
            if (frequency_table[i]->frequency < min1value)
            {
                min1 = i;
                min1value = frequency_table[i]->frequency;
            }
        }
        for (i = 0; i < ASCII_CHARS; i++)
        {
            if (frequency_table[i] == NULL || i == min1)
                continue;
            if (i == min2)
                continue;
            if (frequency_table[i]->frequency < min2value)
            {
                min2 = i;
                min2value = frequency_table[i]->frequency;
            }
        }
        root = calloc(1, sizeof(struct node));
        strcpy(frequency_table[min1]->code, "0");
        root->left = frequency_table[min1];
        strcpy(frequency_table[min2]->code, "1");
        root->right = frequency_table[min2];
        root->frequency = frequency_table[min1]->frequency + frequency_table[min2]->frequency;
        root->c = '\0';
        frequency_table[min1]->parent = root;
        frequency_table[min2]->parent = root;
        frequency_table[min1] = root;
        frequency_table[min2] = (struct node *)NULL;

        if (root->frequency == total_frequency)
            break;
        //        for (i = 0; i < ASCII_CHARS; i++)
        //        {
        //            if (frequency_table[i] == NULL)
        //                continue;
        //            printf("%c - %d\n", i + ' ', frequency_table[i]->frequency);
        //        }
        //        printf("============================================\n");
    }
    return root;
}

void fill_huffman_codes(struct node *leaf_nodes[], char *huffman_codes[])
{
    int i = 0;
    struct node *parent;
    for (i = 0; i < ASCII_CHARS; i++)
    {
        if (!leaf_nodes[i])
            continue;
        huffman_codes[i] = calloc(2, sizeof(char));
    }
    for (i = 0; i < ASCII_CHARS; i++)
    {
        if (!leaf_nodes[i])
            continue;
        parent = leaf_nodes[i];
        while (parent->parent)
        {
            strcat(huffman_codes[i], parent->code);
            huffman_codes[i] = realloc(huffman_codes[i], strlen(huffman_codes[i]) + 2);
            parent = parent->parent;
        }
    }
    printf("Huffman code table for given text: \n");
    for (i = 0; i < ASCII_CHARS; i++)
    {
        if (!huffman_codes[i])
            continue;
        strrev(huffman_codes[i]);
        printf("%c - %s\n", i + ' ', huffman_codes[i]);
    }
    printf("===========================================\n");
}

char *encode_text(char *text, char *huffman_codes[])
{
    int i = 0;
    char c;
    char *encoded_text;
    encoded_text = calloc(strlen(huffman_codes[text[0] - ' ']), sizeof(char));
    strcpy(encoded_text, huffman_codes[text[i] - ' ']);
    for (i = 1; i < strlen(text); i++)
    {
        c = text[i];
        encoded_text = realloc(encoded_text, strlen(encoded_text) + strlen(huffman_codes[c - ' ']) + 1);
        strcat(encoded_text, huffman_codes[c - ' ']);
    }
    return encoded_text;
}

char *decode_text(char *encoded_text, struct node *root)
{
    int i = 0;
    char *decoded_text;
    char c;
    struct node *current = root;
    decoded_text = calloc(1, sizeof(char));
    for (i = 0; i < strlen(encoded_text); i++)
    {
        c = encoded_text[i];
        if (c == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
        if (!current->left && !current->right)
        {
            decoded_text = realloc(decoded_text, strlen(decoded_text) + 2);
            strcat(decoded_text, &(current->c));
            //            printf("%c", current->c);
            current = root;
        }
    }
    //    printf("\n");
    return decoded_text;
}

void main(int argc, char *argv[])
{
    struct node *frequency_table[ASCII_CHARS] = {0};
    struct node *leaf_nodes[ASCII_CHARS] = {0};
    char *huffman_codes[ASCII_CHARS] = {0};
    char *encoded_text, *decoded_text;
    int i = 0;
    char text[100];
    char c, choice;
    struct node *root;
    printf("Use pre-determined text?[Y/y]> ");
    scanf("%c", &choice);
    if (choice != 'y' && choice != 'Y' && choice != '\n')
    {
        getchar();
        scanf("%99[^\n]s", &text);
    }
    else
    {
        strcpy(text, "a simple demonstration of huffman encoding and decoding");
    }
    for (i = 0; i < strlen(text); i++)
    {
        c = text[i];
        if (frequency_table[c - ' '] == NULL)
        {
            frequency_table[c - ' '] = calloc(1, sizeof(struct node));
            leaf_nodes[c - ' '] = frequency_table[c - ' '];
        }
        frequency_table[c - ' ']->frequency++;
        frequency_table[c - ' ']->c = c;
    }
    root = construct_huffman_tree(frequency_table, strlen(text), leaf_nodes);
    fill_huffman_codes(leaf_nodes, huffman_codes);
    encoded_text = encode_text(text, huffman_codes);
    printf("Encoded text: \n");
    printf("%s\n", encoded_text);
    printf("Decoded text: \n");
    decoded_text = decode_text(encoded_text, root);
    printf("%s\n", decoded_text);
}
