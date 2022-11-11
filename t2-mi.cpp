#include<iostream>
#include<stdlib.h>

using namespace std;

/*
 * CRC-8 calculation routine as per ETSI EN 302 755.
 *
 * Polynomial used: x^8 + x^7 + x^6 + x^4 + x^2 + 1 (0xD5)
 */
static const uint8_t crc8_table[256] = {
    0x00, 0xd5, 0x7f, 0xaa, 0xfe, 0x2b, 0x81, 0x54, 0x29, 0xfc, 0x56, 0x83,
    0xd7, 0x02, 0xa8, 0x7d, 0x52, 0x87, 0x2d, 0xf8, 0xac, 0x79, 0xd3, 0x06,
    0x7b, 0xae, 0x04, 0xd1, 0x85, 0x50, 0xfa, 0x2f, 0xa4, 0x71, 0xdb, 0x0e,
    0x5a, 0x8f, 0x25, 0xf0, 0x8d, 0x58, 0xf2, 0x27, 0x73, 0xa6, 0x0c, 0xd9,
    0xf6, 0x23, 0x89, 0x5c, 0x08, 0xdd, 0x77, 0xa2, 0xdf, 0x0a, 0xa0, 0x75,
    0x21, 0xf4, 0x5e, 0x8b, 0x9d, 0x48, 0xe2, 0x37, 0x63, 0xb6, 0x1c, 0xc9,
    0xb4, 0x61, 0xcb, 0x1e, 0x4a, 0x9f, 0x35, 0xe0, 0xcf, 0x1a, 0xb0, 0x65,
    0x31, 0xe4, 0x4e, 0x9b, 0xe6, 0x33, 0x99, 0x4c, 0x18, 0xcd, 0x67, 0xb2,
    0x39, 0xec, 0x46, 0x93, 0xc7, 0x12, 0xb8, 0x6d, 0x10, 0xc5, 0x6f, 0xba,
    0xee, 0x3b, 0x91, 0x44, 0x6b, 0xbe, 0x14, 0xc1, 0x95, 0x40, 0xea, 0x3f,
    0x42, 0x97, 0x3d, 0xe8, 0xbc, 0x69, 0xc3, 0x16, 0xef, 0x3a, 0x90, 0x45,
    0x11, 0xc4, 0x6e, 0xbb, 0xc6, 0x13, 0xb9, 0x6c, 0x38, 0xed, 0x47, 0x92,
    0xbd, 0x68, 0xc2, 0x17, 0x43, 0x96, 0x3c, 0xe9, 0x94, 0x41, 0xeb, 0x3e,
    0x6a, 0xbf, 0x15, 0xc0, 0x4b, 0x9e, 0x34, 0xe1, 0xb5, 0x60, 0xca, 0x1f,
    0x62, 0xb7, 0x1d, 0xc8, 0x9c, 0x49, 0xe3, 0x36, 0x19, 0xcc, 0x66, 0xb3,
    0xe7, 0x32, 0x98, 0x4d, 0x30, 0xe5, 0x4f, 0x9a, 0xce, 0x1b, 0xb1, 0x64,
    0x72, 0xa7, 0x0d, 0xd8, 0x8c, 0x59, 0xf3, 0x26, 0x5b, 0x8e, 0x24, 0xf1,
    0xa5, 0x70, 0xda, 0x0f, 0x20, 0xf5, 0x5f, 0x8a, 0xde, 0x0b, 0xa1, 0x74,
    0x09, 0xdc, 0x76, 0xa3, 0xf7, 0x22, 0x88, 0x5d, 0xd6, 0x03, 0xa9, 0x7c,
    0x28, 0xfd, 0x57, 0x82, 0xff, 0x2a, 0x80, 0x55, 0x01, 0xd4, 0x7e, 0xab,
    0x84, 0x51, 0xfb, 0x2e, 0x7a, 0xaf, 0x05, 0xd0, 0xad, 0x78, 0xd2, 0x07,
    0x53, 0x86, 0x2c, 0xf9
};

uint8_t au_crc8(const uint8_t *buffer, size_t len)
{
    unsigned int crc = 0;
    while (len--)
    {   const unsigned int i = (crc ^ *buffer++) & 0xFF;
        crc = (crc8_table[i] ^ (crc << 8)) & 0xFF;
    }
    return crc & 0xFF;
}

/*
 * CRC-32b by Andrey Dyldin
 *
 * Based on "File Verification Using CRC" by Mark R. Nelson in
 * Dr. Dobb's Journal, May 1992, pp. 64-67
 */
static const uint32_t crc32_table[256] = {
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,
    0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,
    0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3,
    0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef,
    0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,
    0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,
    0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4,
    0x0808d07d, 0x0cc9cdca, 0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,
    0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc,
    0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050,
    0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
    0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb, 0x4f040d56, 0x4bc510e1,
    0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5,
    0x3f9b762c, 0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,
    0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd,
    0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,
    0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,
    0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e,
    0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a,
    0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,
    0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,
    0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

uint32_t au_crc32b(const uint8_t *buffer, int size)
{
    uint32_t crc = 0xffffffff;
    int n = (size + 7) / 8;
    const int m = size % 8;
#define STEP { crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ (*buffer)) & 0xFF]; ++buffer; }
    switch(m)
    {   case 0: do { STEP;
        case 7:      STEP;
        case 6:      STEP;
        case 5:      STEP;
        case 4:      STEP;
        case 3:      STEP;
        case 2:      STEP;
        case 1:      STEP;
                   } while (--n > 0);
    }
#undef STEP
    return crc;
}

// TS reader
istream* inbuf;
int tsrd_pos=0;  // Current readed TS position in 188 byte structure
int tsrd_cc=0xFF;// TS continuity counter
int t2pid=4096;  // T2pid
int pidfound=0;
// Function for reading data, checking and removing transport stream header and adaption fields
// Return 1 if all OK
// Return 0 on read error
int get_tsdata(unsigned char* ts_buf, int i_len, int get_pusi)
{ unsigned char ts_header[4];
  int ts_buf_pos=0;
  if(get_pusi && tsrd_pos > 0)
  { inbuf->read((char *)ts_buf,188-tsrd_pos); // Dummy Read TS
    tsrd_pos=0;
    if(inbuf->fail()) { fprintf(stderr,"Can not read packet\n"); exit(-1);}}
// Read data loop
do // While all buffer by len is filled in
 { if(tsrd_pos == 0)
   {// Read TS header
    inbuf->read((char *)ts_header,4);
    tsrd_pos+=4;
    if(inbuf->fail()) { fprintf(stderr,"Can not read packet\n"); exit(-1);}
    if(ts_header[0]!=0x47) // TO DO: Re-sync.
      { fprintf(stderr,"Fatal error in stream\n");
        exit(-1);}
    if(ts_header[1]&0x80)
      { fprintf(stderr,"Transport Error %02x\n",ts_header[1]);
        return 0;}
    if(ts_header[3]&0xC0)
      { fprintf(stderr,"Encrypted %02x\n",ts_header[3]);
        return 0;}
    // Get Pid
    int pid=(ts_header[1])&0x1F;
    pid<<=8;
    pid|=ts_header[2];
    if(pid == 0x1FFF) // Ignore NULL packets
        goto dummyread184;
    if(pid!=t2pid) // Check for PID
      { if(!pidfound) { pidfound=pid; fprintf(stderr,"Found also another PID: %d\n",pidfound); }
        goto dummyread184; }
    if((ts_header[3]&0x30)==0x20) { // Check for empty frames - 10 – adaptation field only, no payload
dummyread184: // Dummy Read TS 184 bytes
        inbuf->read((char *)&ts_buf[ts_buf_pos],184);
        tsrd_pos=0;
        if(inbuf->fail()) { fprintf(stderr,"Can not read packet\n"); exit(-1);}
        continue;}
    // Check for packet continuity
    if(tsrd_cc==0xFF) tsrd_cc=ts_header[3]&0x0F;
    if(tsrd_cc!=(ts_header[3]&0x0F))
      { fprintf(stderr,"TS packet incontinuity on PID %d - expected %02x, found %02x ",pid,tsrd_cc,ts_header[3]&0x0F);
        tsrd_cc=ts_header[3]&0x0F;
        for(int k=0;k<4;k++) fprintf(stderr," %02x",ts_header[k]); fprintf(stderr,"\n");}
    if(tsrd_cc==0x0F) { tsrd_cc=0; } else { tsrd_cc++; }
    if(get_pusi && (ts_header[1]&0x40)!=0x40) // if PUSI required then scroll to it
      { goto dummyread184; }
    // set to end of adaptation field
    int adapt_len=0;
    if((ts_header[3]&0x30)==0x30) // 11 – adaptation field followed by payload
      { inbuf->read((char *)ts_header,1);
        tsrd_pos++;
        if(inbuf->fail()) { fprintf(stderr,"Can not read packet\n"); exit(-1);}
        adapt_len=ts_header[0];} //add adaption offset
    // Read additional one byte as PUSI the first byte of the payload indicates where this new payload unit starts
    if((ts_header[1]&0x40)==0x40) adapt_len++;
    if(adapt_len > 183)
      { fprintf(stderr,"Wrong Adaptation Field length: %02x, calc %d\n",ts_header[0],adapt_len);
        return 0;}
    // Read adaptation field and PUSI byte
    if(adapt_len > 0)
     {  inbuf->read((char *)&ts_buf[ts_buf_pos],adapt_len);
        tsrd_pos+=adapt_len;
        if(inbuf->fail()) { fprintf(stderr,"Can not read packet\n"); exit(-1);}}
    // On PUSI request scroll on prevous ts data till payload start
    if(get_pusi)
     {  if((ts_header[1]&0x40)==0x40)
         {  int pay_fill_len=ts_buf[ts_buf_pos+adapt_len-1];
            if(pay_fill_len > 188-tsrd_pos)
              { fprintf(stderr,"Wrong Payload data start: %d, tsrd_pos %d\n",pay_fill_len,tsrd_pos);
                return 0;}
            // Read dummy data till payload
            inbuf->read((char *)&ts_buf[ts_buf_pos],pay_fill_len);
            tsrd_pos+=pay_fill_len;
            if(inbuf->fail()) { fprintf(stderr,"Can not read packet\n"); exit(-1);}}
        get_pusi=0;}// Reset PUSI request when scroll done
  }// if(tsrd_pos == 0)
    else
     if(tsrd_pos < 4)
      { fprintf(stderr,"Check software, wrong tsrd_pos: %d\n",tsrd_pos);
        return 0;}
  // Read payload
  int rd_len=i_len-ts_buf_pos;
  if(rd_len > 188-tsrd_pos) rd_len=188-tsrd_pos;
  inbuf->read((char *)&ts_buf[ts_buf_pos],rd_len);
  tsrd_pos+=rd_len;
  ts_buf_pos+=rd_len;
  if(tsrd_pos >= 188) tsrd_pos=0;
  if(inbuf->fail()) { fprintf(stderr,"Can not read packet\n"); exit(-1);}
 } while(ts_buf_pos < i_len); // do
return 1;
} // End of get_tsdata

#define MAXT2SIZE 8000 // 6060
// #define MAXT2SIZE 0x5009
// Main part
int main(int argc, char** argv) {
  ostream* outbuf;
  outbuf = &cout;
  inbuf = &cin;
  char syncB[1] = {0x47};
  if(argc!=3) {
    fprintf(stderr, "Usage: %s [PID] [PLP-ID]\n", argv[0]);
    return 1; }
  t2pid=atoi(argv[1]);
  int plpId=atoi(argv[2]);
  int plpfound=-1;
  unsigned char t2crc[4];
  unsigned char* t2packet;
  t2packet = new unsigned char[MAXT2SIZE];
  int t2packet_cc=0xFFF; // T2 continuity counter
  int payload_len;
  int sync_pusi=1;
  int first=1;
  int pcnt=0;
  uint32_t calc_crc;
  uint32_t data_crc;
  // for(int k=0;k<8;k++) get_tsdata(t2packet, 6, 1);// Drop some beginning
  do
  { // Get T2-MI header
    if(!get_tsdata(t2packet, 6, sync_pusi))
      { sync_pusi=1; continue;}
    // Check T2-MI packet type
    if(t2packet[0]&0xCE)
      { fprintf(stderr, "T2-MI packet type not supported %02x ",t2packet[0]);
        for(int k=0;k<6;k++) fprintf(stderr," %02x",t2packet[k]); fprintf(stderr,"\n");
        sync_pusi=1; continue;}
    sync_pusi=0;
    // Check for T2 packet continuity
    if(t2packet_cc==0xFFF) t2packet_cc=t2packet[1];
    if(t2packet_cc!=t2packet[1])
      { fprintf(stderr,"T2 packet incontinuity expected %02x, found %02x\n",t2packet_cc,t2packet[1]);
        t2packet_cc=t2packet[1];}
    if (t2packet_cc==0xFF) { t2packet_cc=0; } else { t2packet_cc++; }
    // Check Payload len
    if(t2packet[5]&0x7)
      { fprintf(stderr, "Payload len needs shifting: %02x",t2packet[5]);
        if(t2packet[5]==0x84)
          { fprintf(stderr, ", possible it is not T2-MI packet");
            for(int k=0;k<6;k++) fprintf(stderr," %02x",t2packet[k]);}
        fprintf(stderr,"\n");}
    payload_len=(t2packet[4]<<5)|(t2packet[5]>>3);
    if(payload_len <11)
      { fprintf(stderr, "Too small payload len: %4d",payload_len);
        sync_pusi=1; continue;}
    if(payload_len+6 > MAXT2SIZE)
      { fprintf(stderr, "T2 Buffer %4d too small for payload: %4d",MAXT2SIZE,payload_len+6);
        sync_pusi=1; continue;}
    // Get T2-MI Payload
    if(!get_tsdata(&t2packet[6],payload_len,0)) sync_pusi=1;
    // Get CRC32
    if(!get_tsdata(t2crc, 4, 0)) sync_pusi=1;
    // CRC32 MPEG calculation
    calc_crc = au_crc32b(t2packet,payload_len+6);
    data_crc = (unsigned int)t2crc[0]; data_crc <<=8;
    data_crc |= (unsigned int)t2crc[1]; data_crc <<=8;
    data_crc |=  (unsigned int)t2crc[2]; data_crc <<=8;
    data_crc |=  (unsigned int)t2crc[3];
    if(calc_crc != data_crc)
      { fprintf(stderr, "T2 CRC32 error, calculated: %08x, found: %08x, len: %d\n",calc_crc,data_crc,payload_len);
        sync_pusi=1; continue;}
    // Check for Baseband Frame
    if(t2packet[0]==0x00)
      {// Check for plpId
       if(t2packet[7]==plpId)
        { // Process Baseband Frame
          int dfl = (t2packet[13]<<5)|(t2packet[14]>>3);
          int syncd =(t2packet[16]<<5)|(t2packet[17]>>3);
          if(syncd==0xFFFF) continue;
          // Check CRC-8/DVB-S2
          unsigned char bbh_crc=au_crc8(&t2packet[9],9);
          if((bbh_crc&0xFE) != (t2packet[18]&0xFE))
            { fprintf(stderr, "BBHeader CRC8 error, calculated: %02x, found: %02x, dfl: %d\n",bbh_crc,t2packet[18],dfl);
              continue;}
          if(dfl > (payload_len-10) || syncd > 186)
            { fprintf(stderr, "Invalid length: syncd %d, dfl %d, payload_len %d\n",syncd,dfl,payload_len);
              continue;}
          if(syncd>0 && !first) outbuf->write((char*)&t2packet[19],syncd);
          first=0;
          do
            { outbuf->write(syncB,1);
              outbuf->write((char*)&t2packet[19+syncd],187);
              syncd+=187;
            } while (syncd < dfl-187);
          if((dfl-syncd)>0)
            { outbuf->write(syncB,1);
              outbuf->write((char*)&t2packet[19+syncd],dfl-syncd);}
        } // End of Process Baseband Frame
      else
        if(plpfound==-1)
          { plpfound=t2packet[7];
            fprintf(stderr,"Found also another PLP: %d\n",t2packet[7]);}
     }// if(t2packet[0]==0x00)
    //fprintf(stderr, ".");
  } while((pcnt) <20);
  return 0;
}
// End of file
