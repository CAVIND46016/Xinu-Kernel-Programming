#include <fs.h>
#if FS

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;


#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    kprintf("ERROR: No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}


/* read in an inode and fill in the pointer */
int fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    kprintf("ERROR: Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    kprintf("ERROR: fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);


  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    kprintf("ERROR: Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    kprintf("ERROR: fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  kprintf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    kprintf("ERROR: Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    //kprintf("ERROR: fs_mkfs getmem failed.\n");
	kprintf("ERROR: getmem failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void fs_print_fsd(void) {

  kprintf("fsd.ninodes: %d\n", fsd.ninodes);
  kprintf("sizeof(struct inode): %d\n", sizeof(struct inode));
  kprintf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  kprintf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      kprintf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      kprintf("\n");
    }
  }
  kprintf("\n");
}
////////////////////////////////////////////////////
////////////////////////////////////////////////////
int fs_open(char *fop, int flags)
{
int i,tempfd,fd;
	struct directory root_dir;
	root_dir = fsd.root_dir;
	//NUmber of nlinks ++
	if(fsd.root_dir.numentries != 0)
	{
		for(i=0;i<root_dir.numentries;i++)
		{
			if(strncmp(fop,root_dir.entry[i].name,FILENAMELEN) == 0)
			{
				for(tempfd = 0;tempfd < NUM_FD; tempfd++)
                            {
                                if(next_open_fd == -1) return SYSERR;					
                                 if(oft[next_open_fd].state == FSTATE_OPEN )
					{
						kprintf("\nERROR : File already open");
						return tempfd;
					}
                             }
				 //no open file 
				oft[next_open_fd].fileptr = 0;
				oft[next_open_fd].de = getmem(sizeof(struct dirent));
				oft[next_open_fd].de->inode_num = root_dir.entry[i].inode_num;					

				oft[next_open_fd].state = FSTATE_OPEN; //changed
				oft[next_open_fd].flags = flags;//mode of file like read, write or read/write
				
				strncpy(oft[next_open_fd].de->name,root_dir.entry[i].name,FILENAMELEN);
				fs_get_inode_by_num(dev0,oft[next_open_fd].de->inode_num,&oft[next_open_fd].in); //check for errors
				fd = next_open_fd;
				for(tempfd = 0;tempfd < NUM_FD; tempfd++)
				{
					if(oft[tempfd].state == FSTATE_CLOSED)
						next_open_fd = tempfd;
				}
				if(tempfd == NUM_FD) 
					next_open_fd = -1; //if no open fd found
			}
		}	
	}
	return fd;

}////////////////////////
////////////////////////
int fs_close(int fcl)
{
fs_put_inode_by_num(dev0,oft[fcl].in.id,&oft[fcl].in);
	freemem(oft[fcl].de,sizeof(struct dirent));
	oft[fcl].state = FSTATE_CLOSED;
	return OK;
}
/////////////////
/////////////////////
int fs_create(char *fc, int mode)
{
int i;
	struct directory root_dir;
	struct inode *inde;
	root_dir = fsd.root_dir;
	if(mode != O_CREAT)
		{
		kprintf("ERROR :Wrong Mode !Only create mode possible");
		return SYSERR;
		}

	if(fsd.root_dir.numentries != 0)
	{
		for(i=0;i<fsd.root_dir.numentries;i++)
		{
				if(strncmp(fc,fsd.root_dir.entry[i].name,FILENAMELEN )==0)
			{	
				kprintf("ERROR:File already created !!");
				return -1;
			}
		}
	}
	inde = getmem(sizeof(struct inode));
	inde->id = fsd.inodes_used +1;
	fsd.inodes_used=fsd.inodes_used+1;
	inde->type = INODE_TYPE_FILE;
	inde->nlink = 1;
	inde->device = dev0;
	inde->size = 0;
	for(i=0;i<INODEBLOCKS;i++)
		inde->blocks[i] = -1;
	fs_put_inode_by_num(dev0,inde->id,inde);
	fsd.root_dir.entry[fsd.root_dir.numentries].inode_num  = inde->id;
	strncpy(fsd.root_dir.entry[root_dir.numentries].name,fc,FILENAMELEN);
	fsd.root_dir.numentries++; 
	return fs_open(fc,O_RDWR);

}
////////////////////
//////////////////
int fs_seek(int fs, int offset)
{
int fileptr;
	fileptr = oft[fs].fileptr;
	fileptr += offset;
	if(fileptr > oft[fs].in.size || fileptr < 0 )
		return SYSERR;
	oft[fs].fileptr = fileptr;
	return OK;
}
//////////////////////
///////////////////
int fs_read(int fr, void *buf, int nbytes)
{
int read = 0, stored = 0, rdbytes;
	int block_index,block_num,offset;
  block_index = oft[fr].fileptr / dev0_blocksize;
	block_num = oft[fr].in.blocks[block_index];
	offset = oft[fr].fileptr % dev0_blocksize;
	stored = nbytes;
  if(oft[fr].flags != O_RDONLY && oft[fr].flags != O_RDWR)
  {
                kprintf("\nERROR:File not opened in read mode");
                return SYSERR;
        }
	if(oft[fr].state == FSTATE_CLOSED)
        {
                kprintf("\nERROR!! The file is closed");
                return SYSERR;
        }
	while(read < nbytes)
	{
		if(oft[fr].fileptr >= oft[fr].in.size)
			return read;
		if(block_num == -1)
		{ 
			kprintf("\nERROR reading block "); 
			return SYSERR;
		}
		if(dev0_blocksize - offset >  stored)
		{
			rdbytes = stored;
		}	
		else
		{
			rdbytes = dev0_blocksize - offset;
		}		
		if(bs_bread(dev0,block_num, offset, &buf[read], rdbytes) == SYSERR)
		{
			kprintf("\nERROR!! In reading file");
			oft[fr].fileptr += read;
			return read;
		}
		read += rdbytes;
		oft[fr].fileptr += rdbytes;
		stored -= rdbytes;
		block_index++;
		block_num = oft[fr].in.blocks[block_index];
		offset = 0;
	}

        return read;

}
////////////////////////////
///////////////////////
int fs_write(int fw, void *buf, int nbytes)
{
	int write, written = 0, writbytes = 0;
	int block_index,block_num,offset;
	struct inode in;
	
	if(oft[fw].flags != O_WRONLY && oft[fw].flags != O_RDWR)
	{
		kprintf("\nERROR:File not opened in write mode");
		return SYSERR;
	}
	if(oft[fw].state == FSTATE_CLOSED)
	{
		kprintf("\nERROR!! The file is closed");
		return SYSERR;
	}
	in = oft[fw].in;
	in.size = in.size - (in.size - oft[fw].fileptr);
	block_index = oft[fw].fileptr / dev0_blocksize;
        block_num = oft[fw].in.blocks[block_index];
	if(block_num == -1)
	{
		block_num = fs_getfreeblock();
		oft[fw].in.blocks[block_index] = block_num;
	}
	offset = oft[fw].fileptr % dev0_blocksize;
	write = nbytes;
	while (write > 0)
	{
		fs_setmaskbit(block_num);
		if(dev0_blocksize-offset > write)
		{
			writbytes = write;
		}
		else
		{
			writbytes = dev0_blocksize - offset;
		}
		if(bs_bwrite(dev0,block_num,offset,&buf[written],writbytes) == SYSERR)
		{
			kprintf("\nERROR!!In file writing");
			oft[fw].in.size += written;
			return written;
		}
		written += writbytes;
		write -= (writbytes);
		block_index++;
		oft[fw].in.blocks[block_index] = fs_getfreeblock();
		block_num =  oft[fw].in.blocks[block_index]; 
		offset = 0;
	}
	oft[fw].in.size += written; 
	oft[fw].fileptr = oft[fw].in.size;
	return written;
}
////////////////////////////
/* filesystem functions */
int fs_mount(int device)
{
	intmask	mask;			
	struct	nmentry	*namptr;	
	int32	i;			

        mask = disable();

	

	if ( (isbaddev(device)) || (nnames >= NNAMES) ) {
		restore(mask);
		return SYSERR;
	}

	

	namptr = &nametab[nnames];	

	namptr->ndevice = device;	

        nnames++;			

	restore(mask);
	return OK;
}



///////////////////////////////////////////
/*get a free block by checking the mask bit*/
int fs_getfreeblock()
{
	int i;
	for(i=0;i<dev0_numblocks;i++)
	{
		if(fs_getmaskbit(i) == 0)
			return i;
	}
	return -1;
}
#endif /* FS */

