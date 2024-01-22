#ifndef SD_DATA_H
#define SD_DATA_H

class Logger{
  public:
    char* current_status_reading_file;
    char* current_status_writing_file;
    char* current_imaging_data_reading_file;
    char* current_imaging_data_writing_file = 0;

    unsigned int reading_status_file_pointer;
    unsigned int reading_imaging_file_pointer = 0;

    Logger();

    void switch_status_file();
    void switch_imaging_file();

    // void sdReadSatStatusPacket(HealthData *healthData, unsigned int index);
    // void sdWriteSatStatusPacket(unsigned long int t,
    //   float bv, float bcu, float bch, float bt, float ti, float te,
    //   unsigned long int sd, uint8_t* rasp_data);

    // void sdReadSatImagingDataPacket(ImagingData *imagingData, unsigned int index);
    // void sdWriteSatImagingDataPacket(ImagingData im);
};

#endif