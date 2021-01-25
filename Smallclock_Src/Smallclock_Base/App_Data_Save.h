#ifndef APP_DATA_SAVE_H
#define APP_DATA_SAVE_H

#include <string>
#include <fstream>

using namespace std;

class App_Data_Save
{
private:
    string m_str_data_path;
    ifstream* m_if_data_read = 0;
    ofstream* m_of_data_save = 0;

    bool open_infile()
    {
        m_if_data_read = new ifstream(m_str_data_path);
        return m_if_data_read->is_open();
    }
    void close_infile()
    {
        if(m_if_data_read != 0)
        {
            m_if_data_read->close();
            delete m_if_data_read;
            m_if_data_read = 0;
        }
    }

    bool open_outfile()
    {
        m_of_data_save = new ofstream(m_str_data_path);
        return m_of_data_save->is_open();
    }
    void close_outfile()
    {
        if(m_of_data_save != 0)
        {
            m_of_data_save->close();
            delete m_of_data_save;
            m_of_data_save = 0;
        }
    }
public:
    App_Data_Save(string data_path)
    {
        m_str_data_path = data_path;
    }
    ~App_Data_Save()
    {
        close_infile();
        close_outfile();
    }

    bool start_read()
    {
        return open_infile();
    }
    template<typename Tvalue>
    Tvalue read_next_data()
    {
        Tvalue data;
        m_if_data_read->read((char*)(&data), sizeof (data));
        return data;
    }
    string read_next_data()
    {
        string data;

        int read_data = 0;
        while (true)
        {
            read_data = m_if_data_read->get();
            if(read_data == 0 || read_data == -1)
            {
                break;
            }
            else
            {
                data += char(read_data);
            }
        }
        return data;
    }
    void finish_read()
    {
        close_infile();
    }

    bool start_write()
    {
        return open_outfile();
    }
    template<typename Tvalue>
    void write_next_data(Tvalue value)
    {
        m_of_data_save->write((char*)(&value), sizeof(value));
    }
    void write_next_data(string value)
    {
        char char_zero = 0;
        m_of_data_save->write((char*)(value.c_str()), sizeof(char) * value.size());
        m_of_data_save->write(&char_zero, sizeof(char_zero));
    }
    void finish_write()
    {
        close_outfile();
    }


};

#endif // APP_DATA_SAVE_H
