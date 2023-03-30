#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <chrono>
#include <thread>

#define MAZE_PATH "\033[48;5;235m  \033[0m"
#define MAZE_WALL "\033[48;5;240m|_\033[0m"
#define MAZE_ENTRY "\033[48;5;45m >\033[0m"
#define MAZE_EXIT "\033[48;5;46m >\033[0m"
#define MAZE_BREADCRUMBS "\033[48;5;235m::\033[0m"
#define MAZE_RUNNER "\033[48;5;196m<>\033[0m"

// Matriz de char representnado o labirinto
char **maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t
{
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento:

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos:
//    valid_positions.size();
//
// Retornar o elemento no topo:
//  valid_positions.top();
//
// Remover o primeiro elemento do vetor:
//    valid_positions.pop();

// Função que le o labirinto de um arquivo texto, carrega em
// memória e retorna a posição inicial
pos_t load_maze(const char *file_name)
{
	pos_t initial_pos;

	// FILE *F = fopen(file_name, "r");
	// std::cout << file_name << std::endl;

	std::ifstream maze_file;
	maze_file.open(file_name);
	// Abre o arquivo para leitura (fopen)

	// Le o numero de linhas e colunas (fscanf)
	// e salva em num_rows e num_cols

	std::string first_line;
	std::getline(maze_file, first_line);
	sscanf(first_line.c_str(), "%d %d", &num_rows, &num_cols);

	// Aloca a matriz maze (malloc)
	maze = (char **)malloc(sizeof(char *) * num_rows);

	for (int i = 0; i < num_rows; ++i)
		// Aloca cada linha da matriz
		maze[i] = (char *)malloc(sizeof(char) * num_cols);

	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos

			char read_char;

			while (true)
			{
				read_char = maze_file.get();

				if (read_char == '#' || read_char == 'x' || read_char == 's' || read_char == 'e')
				{
					maze[i][j] = read_char;

					if (read_char == 'e')
					{
						struct pos_t e_pos = {i, j};
						initial_pos = e_pos;
					}

					break;
				}

				if (!maze_file)
				{
					break;
				}
			}
		}
	}

	return initial_pos;
}

// Função que imprime o labirinto
void print_maze()
{

	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{

			switch (maze[i][j])
			{
			case '#':
				printf(MAZE_WALL);
				break;

			case 'x':
				printf(MAZE_PATH);
				break;

			case 'e':
				printf(MAZE_ENTRY);
				break;

			case 's':
				printf(MAZE_EXIT);
				break;

			case '.':
				printf(MAZE_BREADCRUMBS);
				break;

			case 'c':
				printf(MAZE_RUNNER);
				break;

			default:
				// printf("not found");
				break;
			}
		}
		printf("\n");
	}
}

// Função que imprime o labirinto
void update_maze(struct pos_t old_pos, struct pos_t new_pos)
{

	std::cout << "\0337" << std::flush;
	std::cout << "\033[H\033[" << old_pos.i + 1 << ";" << old_pos.j * 2 + 1 << "H" << MAZE_BREADCRUMBS << std::flush;
	std::cout << "\033[H\033[" << new_pos.i + 1 << ";" << new_pos.j * 2 + 1 << "H" << MAZE_RUNNER << std::flush;
	std::cout << "\0338" << std::flush;
}

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos)
{
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	// Marcar a posição atual com o símbolo '.'
	maze[pos.i][pos.j] = 'c';

	// Limpa a tela

	// Imprime o labirinto
	// print_maze();

	/* Dado a posição atual, verifica quais sao as próximas posições válidas
		Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		cada uma delas no vetor valid_positions
			- pos.i, pos.j+1
			- pos.i, pos.j-1
			- pos.i+1, pos.j
			- pos.i-1, pos.j
		Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	*/

	if (((pos.j + 1) < num_cols && maze[pos.i][pos.j + 1] == 's') ||
			((pos.j - 1) >= 0 && maze[pos.i][pos.j - 1] == 's') ||
			((pos.i + 1) < num_rows && maze[pos.i + 1][pos.j] == 's') ||
			((pos.i - 1) >= 0 && maze[pos.i - 1][pos.j] == 's'))
	{
		return true;
	}

	if ((pos.j + 1) < num_cols && maze[pos.i][pos.j + 1] == 'x')
	{
		struct pos_t new_pos = {pos.i, pos.j + 1};
		valid_positions.push(new_pos);
	}
	if ((pos.j - 1) >= 0 && maze[pos.i][pos.j - 1] == 'x')
	{
		struct pos_t new_pos = {pos.i, pos.j - 1};
		valid_positions.push(new_pos);
	}
	if ((pos.i + 1) < num_rows && maze[pos.i + 1][pos.j] == 'x')
	{
		struct pos_t new_pos = {pos.i + 1, pos.j};
		valid_positions.push(new_pos);
	}
	if ((pos.i - 1) >= 0 && maze[pos.i - 1][pos.j] == 'x')
	{
		struct pos_t new_pos = {pos.i - 1, pos.j};
		valid_positions.push(new_pos);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	// Verifica se a pilha de posições nao esta vazia
	// Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
	// Caso contrario, retornar falso
	if (!valid_positions.empty())
	{
		maze[pos.i][pos.j] = '.';

		pos_t next_position = valid_positions.top();
		valid_positions.pop();

		update_maze(pos, next_position);

		return walk(next_position);
	}
	return false;
}

int main(int argc, char *argv[])
{
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);

	printf("\033[2J");
	printf("\033[H");
	print_maze();

	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	print_maze();

	// Tratar o retorno (imprimir mensagem)
	if (exit_found)
	{
		std::cout << "Saída do labirinto foi encontrada!!!" << std::endl;
	}
	else {
		std::cout << "ERRO!!! Saída não foi encontrada." << std::endl;
	}

	return 0;
}
