//#ifndef COMMAND_DO_IN_THREAD_H
//#define COMMAND_DO_IN_THREAD_H

//#define CHAR_ARR_BUF_SIZE 1024

//#include <stdio.h>
//#include <iostream>
//#include <thread>
//#include <signal.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <memory.h>

//using namespace std;

//template <typename ClassType>
//class Command_Do_In_Thread
//{
//public:
//    Command_Do_In_Thread(string command,
//                         ClassType *class_value,
//                         void (ClassType::*do_when_thread_exit)() = 0,
//                         void (ClassType::*do_when_thread_be_ended)() = 0)
//    {
//        m_str_command = command;
//        m_classtype_class = class_value;
//        m_function_do_when_thread_exit = do_when_thread_exit;
//        m_function_do_when_thread_be_ended = do_when_thread_be_ended;

//        run_the_command();
//    }

//    void end_the_command()
//    {
//        m_is_command_be_ended = true;

//        run_function_do_when_thread_be_ended();
//    }

//private:
//    bool m_is_command_be_ended = false;
//    bool m_is_command_finish = false;
//    char m_char_arr_buf_command_out[CHAR_ARR_BUF_SIZE];
//    string m_str_command;
//    ClassType *m_classtype_class;
//    void (ClassType::*m_function_do_when_thread_exit)();
//    void (ClassType::*m_function_do_when_thread_be_ended)();

//    FILE* m_file_command_popen;
//    int m_int_popen_command_file;

//    thread *m_thread_read_the_data;

//    void run_the_command()
//    {
//        if(m_str_command.size() > 0) //command isn't void
//        {
//            if((m_file_command_popen = popen(m_str_command.c_str(), "r")) == NULL)
//            {
//                run_function_do_when_thread_exit();
//            }
//            else
//            {
//                m_int_popen_command_file = fileno(m_file_command_popen);
//                fcntl(m_int_popen_command_file, F_SETFL, O_NONBLOCK);
//                m_thread_read_the_data = new class thread(&Command_Do_In_Thread::read_the_command_data,
//                                                          this);
//            }
//        }
//        else
//        {
//            run_function_do_when_thread_exit();
//        }
//    }

//    void read_the_command_data()
//    {
//        ssize_t length_read_data = 0;
//        while (m_is_command_be_ended == false)
//        {
//            memset(m_char_arr_buf_command_out, 0, sizeof(m_char_arr_buf_command_out));

//            /* Read NBYTES into BUF from FD.  Return the
//               number read, -1 for errors or 0 for EOF.  */
//            length_read_data = read(m_int_popen_command_file,
//                                    m_char_arr_buf_command_out,
//                                    sizeof(m_char_arr_buf_command_out) - 16);
//            if(length_read_data > 0)
//            {
//                cout << m_char_arr_buf_command_out;
//            }
//            else if(length_read_data == -1)
//            {
//                //1 microsecond = 1/1 000 000 second
//                usleep(10000);
//            }
//            else
//            {
//                break;
//            }
//        }

//        close(m_int_popen_command_file);
//        pclose(m_file_command_popen);
//        m_file_command_popen = 0;
//        m_int_popen_command_file = 0;

//        if(m_is_command_be_ended == false)
//        {
//            run_function_do_when_thread_exit();
//        }
//    }

//    void run_function_do_when_thread_exit()
//    {
//        if(m_function_do_when_thread_exit != 0)
//        {
//            (m_classtype_class->*m_function_do_when_thread_exit)();
//        }
//        clean_the_data_in_class();
//    }
//    void run_function_do_when_thread_be_ended()
//    {
//        if(m_function_do_when_thread_be_ended != 0)
//        {
//            (m_classtype_class->*m_function_do_when_thread_be_ended)();
//        }
//        clean_the_data_in_class();
//    }

//    void clean_the_data_in_class()
//    {
//        m_classtype_class = 0;
//        m_function_do_when_thread_exit = 0;
//        m_function_do_when_thread_be_ended = 0;
//        m_thread_read_the_data = 0;
//    }
//};

//#undef CHAR_ARR_BUF_SIZE

//#endif // COMMAND_DO_IN_THREAD_H
