#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

inline void print_table(std::vector<std::string> header,
						std::vector<std::vector<std::string>> table,
						std::vector<std::string> footer)
{
	//--- PARAMETERS ---//
	int columns = header.size();
	std::vector<int> widths;
	int tot_width = 0;
	for (int c = 0; c < columns; c++)
	{
		int width = std::max(header[c].length(), footer[c].length());
		for (auto &row : table)
		{
			width = std::max(width, (int)row[c].length());
		}
		widths.push_back(width);
		tot_width += width;
	}
	//--- ---//

	//--- PRINTERS ---//
	auto print_divider = [=]()
	{ std::cout << std::string(tot_width + (columns + 1) * 2, '=') << std::endl; };

	auto print_row = [=](std::vector<std::string> row)
	{
		std::cout << std::left << "  " << std::setw(widths[0]) << row[0] << std::right;
		for (int c = 1; c < row.size(); c++)
		{
			std::cout << std::setw(widths[c] + 2) << row[c];
		}
		std::cout << std::endl;
	};
	//--- ---//

	print_divider();
	print_row(header);
	print_divider();
	for (auto &row : table)
	{
		print_row(row);
	}
	print_divider();
	print_row(footer);
	print_divider();
}

inline void print_results(int N, int vals[], int cfrs[], int clear_sum, int cipher_sum)
{
	std::vector<std::vector<std::string>> table;
	for (int i = 0; i < N; i++)
	{
		table.push_back({std::to_string(i), std::to_string(vals[i]), std::to_string(cfrs[i])});
	}
	print_table({"User", "Clear", "Cipher"}, table, {"Sum", std::to_string(clear_sum), std::to_string(cipher_sum)});
}