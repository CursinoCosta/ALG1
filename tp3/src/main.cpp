#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <unordered_map>

using namespace std;

#define _                         \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);

struct Cell
{
    int value;
    bool top_adj, bottom_adj, left_adj, right_adj;

    Cell() {}
    Cell(int brightness, bool top, bool bottom, bool left, bool right)
    {
        value = brightness;
        top_adj = top;
        bottom_adj = bottom;
        left_adj = left;
        right_adj = right;
    }
};

struct Hash_2
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &v) const
    {
        return hash<T1>()(v.first) ^ hash<T2>()(v.second) << 1;
    }
};

struct Hash_3
{
    template <class T1, class T2, class T3>
    size_t operator()(const tuple<T1, T2, T3> &v) const
    {
        return hash<T1>()(get<0>(v)) ^ hash<T2>()(get<1>(v)) ^ hash<T3>()(get<2>(v)) << 1;
    }
};

int R, C, K;
typedef unordered_map<pair<int, int>, Cell, Hash_2> unordered_map_cells;
unordered_map_cells grid;

typedef unordered_map<tuple<int, int, int>, int, Hash_3> unordered_map_3;
unordered_map_3 subproblem;
unordered_map_3 children;

// vector<vector<Cell>> grid;
// vector<vector<vector<Dp>>> dp;
vector<pair<int, int>> previous;

bool set_true(unsigned int number, int x)
{
    return (number >> x) & 1;
}

vector<int> valid_masks(int line)
{
    vector<int> possible_masks;

    for (int i = 0; i < pow(2, C); i++)
    {
        int skip_line = 0;
        for (int j = 0; j < C; j++)
        {
            if (set_true(i, j))
            {
                if ((grid[{line, C - j}]).value == -1)
                {
                    skip_line = 1;
                }
                if ((grid[{line, C - j}]).right_adj == 1)
                {
                    if (j == 0)
                    {
                        if (set_true(i, C - 1))
                        {
                            skip_line = 1;
                        }
                    }
                    else if (set_true(i, j - 1))
                    {
                        skip_line = 1;
                    }
                }
            }
        }
        if (skip_line == 0)
        {
            possible_masks.emplace_back(i);
        }
        skip_line = 0;
    }
    return possible_masks;
}

int get_sum_brightness(int line, int bitmask)
{
    int sum = 0;
    for (int i = 0; i < C; i++)
    {
        if (set_true(line, bitmask))
        {
            sum += grid[{line, C - i}].value;
        }
    }
    return sum;
}

bool adjacency_test(int line, int bitmask1, int bitmask2)
{
    for (int i = 0; i < C; i++)
    {
        if (set_true(bitmask1, i) && ((grid[{line, C - i}].top_adj) == 1) && (set_true(bitmask2, i)))
        {
            return false;
        }
    }
    return true;
}

int dp_f(int bitmask, int bitmask_1, int index_row)
{
    if (subproblem.count({index_row, bitmask, bitmask_1}))
    {
        return subproblem[{index_row, bitmask, bitmask_1}];
    }

    if (index_row == 1)
    {
        if (adjacency_test(1, bitmask_1, bitmask))
        {
            return get_sum_brightness(1, bitmask_1);
        }
        else
        {
            return -99;
        }
    }
    vector<int> possible_bitmasks = valid_masks(index_row);

    int next_row = index_row++;
    if (index_row == R)
    {
        next_row = 1;
    }
    int max_brightness = 0;
    int select;

    for (int m : possible_bitmasks)
    {
        if (adjacency_test( m, bitmask,index_row))
        {
            int answer = dp_f(next_row, m, bitmask_1) + get_sum_brightness(index_row, m);
            if (answer > max_brightness)
            {
                max_brightness = answer;
                select = m;
            }
        }
    }

    subproblem[{index_row, bitmask, bitmask_1}] = max_brightness;
    children[{index_row - 1, bitmask, bitmask_1}] = select;
    return max_brightness;
}

pair<int, int> first_line_dp()
{
    vector<int> line_1 = valid_masks(1);
    int max = 0;
    int chosen;

    for (int mask : line_1)
    {
        int answer = dp_f(mask, mask, 2);
        if (answer > max)
        {
            max = answer;
            chosen = mask;
        }
    }
    subproblem[{1, chosen, chosen}] = max;
    return {max, chosen};
}

int main()
{
    // cout << "Número de linhas (máximo = 600) e colunas (máximo = 6) e a quantidade de cristais: ";
    cin >> R >> C >> K;

    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            grid[{i, j}].value = -1;
            grid[{i, j}].top_adj = 0;
            grid[{i, j}].bottom_adj = 0;
            grid[{i, j}].left_adj = 0;
            grid[{i, j}].right_adj = 0;
            // int brightness, bool top, bool bottom, bool left, bool right
        }
    }

    // cout << "Coordenadas, brilho e adjacência (direita, cima, esquerda, baixo) dos cristais:" << endl;
    int i_aux, j_aux;
    for (int i = 0; i < K; ++i)
    {
        cin >> i_aux >> j_aux >> grid[{i_aux, j_aux}].value >> grid[{i_aux, j_aux}].right_adj >> grid[{i_aux, j_aux}].top_adj >>
            grid[{i_aux, j_aux}].left_adj >> grid[{i_aux, j_aux}].bottom_adj;
    }

    // int best_mask =
    // int max_value = dp[best_mask];

    auto result = first_line_dp();

    int linha = 1;
    int mask = result.second;
    int mask_1 = mask;
    int aux = 0;
    while (linha <= R)
    {
        for (int i = 0; i < C; i++)
        {
            if (set_true(mask, i))
            {
                aux++;
                i_aux = linha;
                j_aux = C - i;
                previous.push_back({i_aux, j_aux});
            }
        }
        mask = children[{linha, mask, mask_1}];
        linha++;
    }

    cout << aux << " " << result.first << endl;

    for (auto c : previous)
    {
        cout << c.first << " " << c.second << endl;
    }

    // vector<pair<int, int>> solution = reconstruct_solution(best_mask);

    // printf("%d %d", solution.size(), max_value);

    // cout << "Cristais usados:" << endl;

    return 0;
}
