#pragma once
#include <iostream>
class Book
{
private:
	char* title;
	char * author;
	unsigned quantity;
public:

	Book(char * t, char* a, int q = 0)
	{
		setTitle(t);
		setAuthor(a);
		setQuantity(q);
	}

	char* getTitle() const
	{
		return title;
	}
	void setTitle(char *tit)
	{
		if (tit != "") title = tit;
		else
			throw std::invalid_argument("Book's title field can not be empty");
	}

	char* getAuthor() const
	{
		return author;
	}
	void setAuthor(char* aut)
	{
		if (aut != "") author = aut;
		else 
			throw std::invalid_argument("Book's authot field can not be empty");
	}

	unsigned getQuantity() const
	{
		return quantity;
	}

	void setQuantity(unsigned q)
	{
		quantity = q;
	}
};