Project 4 Notes

Requirements:
	550 lines of code
	Copious use of STL where we permit
	If you need to define INLINE comparison op. , place them in your header files
	Think about algorithms and data structure
	Simplest data structures
	Test each class independently of the others that depend on it
	Meet big O requirements for each function
	Use STL first and then come back

Turn in:
	MyHash.h
		Resizable hash table map
	Tokenizer.cpp
	WordList.cpp
	Translator.cpp
	Decrypter.cpp
	report.docx
		Known bugs
		Description of data structures and algorithms
		Pseudocode
		Whether or not satisfy big-O requirement

Introduction
	Plaintext letter map to a unique letter of ciphertext letters
	Ciphertext letter map to a unique letter of plaintext letters

One possible algorithm
	int getMaxLength(const vector<string>& v)
	{
		int max = 0;
		for (int i = 0; i < v.size(); i++)
		{
			if (words[i].size() > max)
				max = words[i].size();
		}
		return max;
	}

	bool insertTrans(string w, string p)
	{
		for (int i = 0; i < w.size(); i++)
		{
			if (!contains(w[i], p[i]))
				this.insert(w[i], p[i]);
			if (contains(w[i]) && w[i].trans != p[i])
				return false;
		}
		return true;
	}

	string crack(string ciphereText, vector<string>& output)
	{
		output = "";
		vector<string> words = tokenize(ciphereText);
		int maxLength = getMaxLength(words);
		string w;
		for (int i = 0; i < words.size(); i++)
		{
			if (!chosen && words[i].size() == max)
				w = words[i];
		}
		string p = translate(w, currentMapTable);   // contain all ? in the beginning
		w.chosen = true;
		list<string> C;
		if (currentMapTable.empty())    // all potential
			C.insert(WordList);
		else
		{
			if (WordList.contains(p))
			C.insert(p);
			if (C.empty())
				return;
		}
		MyHash temp =  currentMapTable;
		int i = 0;
		while (i < C.size())
		{
			if (!temp.insertTrans(w, C[i])) // w is actually translated into a word p.
			{
				delete temp;
				i++;
			}
		}
		crackHelper(string cipherText, currentMapTable);   // use this map table to translate the rest
		...
		evaluate
		...
	}


What do you need to do?
	Class 1: MyHash
		Must not use STl.
	Class 2: TokenizerImpl
	Class 3: WordListImpl
		Use MyHash.
	Class 4: TranslatorImpl
	Class 5: DecrypterImpl
		Use your classes.
		Must present the decryptions in alphabetical order.
		Must use the algorithm above.
		Must not use STl.


MyHash.h
	const int NUM_BUCK = 100;
	template <class KeyType, class ValueType>
	class MyHash
	{
	public:
		MyHash(double maxLoadFactor = 0.5);
		~MyHash();
		void reset();
		void associate(const KeyType& key, const ValueType& value);
		const ValueType* find(const KeyType& key) const;
		ValueType* find(const KeyType& key);
		int getNumItems() const;
		double getLoadFactor() const;
	private:
		struct Bucket
		{
			Bucket()
			{
				slot = -1;
				m_head = nullptr;
			}
			~Bucket()
			{
				Node* p = m_head;
				while (p != nullptr)
				{
					Node* temp = p;
					p = p->next;
					delete temp;
				}
				delete m_head;
			}
			int slot;
			Node* m_head;
		}
		struct Node
		{
			Node()
			{ }
			Node(KeyType k, ValueType v)
			{
				key = k;
				value = v;
				next = nullptr;
			}
			KeyType key;
			ValueType value;
			Node* next;
		}
		unsigned int hash(const KeyType& key);
		int m_size;
		int m_nItems;
		double m_maxLF;
		Bucket* m_buckets;
		void swap(const MyHash& mh1, const MyHash& mh2);
	};

	template <class KeyType, class ValueType>
	MyHash<class KeyType, class ValueType>::MyHash(double maxLoadFactor = 0.5)
	{
		m_maxLF = maxLoadFactor;
		if (maxLoadFactor <= 0)
			m_maxLF = 0.5;
		else if (maxLoadFactor > 2)
			m_maxLF = 2.0;
		m_size = NUM_BUCK;
		m_nItems = 0;
		m_buckets = new Bucket[NUM_BUCK];
	}

	template <class KeyType, class ValueType>
	MyHash<class KeyType, class ValueType>::~MyHash()
	{
		delete [] m_buckets;
	}

	template <class KeyType, class ValueType>
	void MyHash<class KeyType, class ValueType>::reset()
	{
		MyHash<class KeyType, class ValueType>* temp = new MyHash(getLoadFactor());
		swap(*this, *temp);
		delete temp;
	}

	template <class KeyType, class ValueType>
	void MyHash<class KeyType, class ValueType>::associate(const KeyType& key, const ValueType& value)
	{
		Node* n = new Node;
		n->key = key;
		n->value = value;
		n->next = nullptr;
		unsigned int hash(const KeyType& key);
		int bucket = hash(key);
		Node* p = m_buckets[bucket]->m_head;
		while(p != nullptr && p->next != nullptr)
			p = p->next;
		p->next = n;
		m_nItems++;
		if (getLoadFactor > m_maxLF)
		{
			// allocate new bigger dynamic array
			Bucket* newBuckets = new Bucket[2 * NUM_BUCK];
			// move
			moveTo(newBuckets);
			m_buckets = newBuckets;
		}

	}


	template <class KeyType, class ValueType>
	const ValueType* MyHash<class KeyType, class ValueType>::find(const KeyType& key) const
	{
		ValueType* vp = new ValueType;
		unsigned int hash(const KeyType& key);
		int bucket = hash(key);
		Node* p = m_buckets[bucket]->m_head;
		while (p != nullptr)
		{
			if  (p->key == key)
				vp = &(p->value);
			else
				p = p->next;
		}
		vp = nullptr;
	}

	// template <class KeyType, class ValueType>
	// ValueType* MyHash<class KeyType, class ValueType>::find(const KeyType& key) const
	// {
	// 	ValueType* vp = new ValueType;
	// 	unsigned int hash(const KeyType& key);
	// 	int bucket = hash(key);
	// 	Node* p = m_buckets[bucket]->m_head;
	// 	while (p != nullptr)
	// 	{
	// 		if  (p->key == key)
	// 			vp = &(p->value);
	// 		else
	// 			p = p->next;
	// 	}
	// 	vp = nullptr;
	// }

	template <class KeyType, class ValueType>
	int MyHash<class KeyType, class ValueType>::getNumItems() const
	{
		return m_nItems;
	}

	template <class KeyType, class ValueType>
	double MyHash<class KeyType, class ValueType>::getLoadFactor() const
	{
		return (m_nItems * 1.0 / NUM_BUCK);
	}

	template <class KeyType, class ValueType>
	inline unsigned int MyHash<class KeyType, class ValueType>::hash(string s)
	{
		return hash<string>()(s);
	}

	template <class KeyType, class ValueType>
	inline unsigned int MyHash<class KeyType, class ValueType>::hash(int i)
	{
		return hash<int>()(i);
	}

	template <class KeyType, class ValueType>
	inline unsigned int MyHash<class KeyType, class ValueType>::hash(char c)
	{
		return hash<char>()(c);
	}

	template <class KeyType, class ValueType>
	void MyHash<class KeyType, class ValueType>::swap(const MyHash<class KeyType, class ValueType>& mh1, const MyHash<class KeyType, class ValueType>& mh2)
	{
		// swap everything
		Bucket* tempB = mh1.m_buckets;
		mh1.m_buckets = mh2.m_buckets;
		mh2.m_buckets = tempB;

		int tempS = mh1.m_size;
		m1.m_size = mh2.m_size;
		mh2.m_size = tempS;

		int tempN = mh1.m_nItems;
		m1.m_nItems = mh2.m_nItems;
		mh2.m_nItems = tempN;

		double tempM = mh1.m_maxLF;
		m1.m_maxLF = mh2.m_maxLF;
		mh2.m_maxLF = tempM;
	}



Tokenizer.cpp
	class Tokenizer
	{
	public:
		Tokenizer(string separators);
		vector<string> tokenize(const string& s) const;
	private:
		vector<char> m_separators;
		vector<string> m_tokens;
	};

	Tokenizer::Tokenizer(string separators)
	{
		for (int i = 0; i < separators.size(); i++)
		{
			m_separators.push_back(separators[i]);
		}
	}

	vector<string> Tokenizer::Tokenizer(const string& s) const
	{
		for (int i = 0; i < s.size(); i++)
		{
			string w;
			if (!find(m_separators.begin(), m_separators.end(), s[i]))
			{
				w += s[i];
			}
			else
			{
				if (w != "")
					m_tokens.push_back(w);
				w = "";
			}
		}
	}




WordList.cpp
#include <iostream>
#include <fstream>
using namespace std;

class WordlistImpl
{
public:
	WordListImpl();
	~WordListImpl();
	bool loadWordList(string dictFilename);
	bool contains(string word) const;
	vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
	MyHash m_wordList;
}

WordListImpl::WordlistImpl()
{}


WordListImpl::~WordListImpl()
{}

bool WordListImpl::loadWordList(string dictFilename);
{
	if (m_wordList.getNumItems() != 0)
		m_wordList.reset();
	ifstream infile(dictFilename);
	if (!infile)
	{
		cerr << "Cannot open " << dictFilename << endl;
	}
	string s;
	while (getline(infile, s))
	{
		int bucket = hash(s);
		m_wordList.associate(bucket, s);
	}
}

bool WordListImpl::contains(string word) const
{
	int key = hash(word);
	if (m_wordList.find(key) != nullptr && *(m_wordList.find(key)) == toLower(word))
		return true;
	return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
	vector<string> result;
	if (cipherWord.size() != currTranslation.size())
		return result;
	for (int i = 0; i < cipherWord.size(); i++)
	{
		if (!isAlpha(cipherWord[i]) && cipherWord[i] != '\'')
			return result;
		if (!isAlpha(currTranslation[i]) && currTranslation[i] != '\'' && currTranslation[i] != '?')
			return result;
	}

	for (int i = 0; i < cipherWord.size(); i++)
	{
		if (isalpha(cipherWord[i]) && !isapha(currTranslation[i]))
			return result;
	}

	string pattern;

}
















	
