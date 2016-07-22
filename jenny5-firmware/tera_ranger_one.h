#ifndef TERA_RANGER_ONE_H
#define TERA_RANGER_ONE_H

class t_tera_ranger_one_controller
{
  private:

    int last_read_distance;
    bool trigger_started;
    unsigned int millis_start;// how many millis were when we trigger

  public:
    t_tera_ranger_one_controller (void);
    void create_init(void);
    void trigger(void);
    bool check_echo(void);
    int get_last_read_distance(void);// cannot be read twice !
      void update_results(char *serial_out);
};

#endif //TERA_RANGER_ONE_H
