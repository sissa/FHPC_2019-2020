# Exploring and using a Lustre FS

 

## exploring/understanding the FS structure 

We will first use the `lfs` utility that allows us to explore how our FS is done
The utility haws many different command we can see  using the help one. We will just use a small subset in this tutorial.

```
[exact@master ~]$ lfs help
Available commands are:
	setstripe
	getstripe
	setdirstripe
	getdirstripe
	mkdir
	rm_entry
	pool_list
	find
	check
	join
	osts
	mdts
	df
	getname
	quotacheck
	quotaon
	quotaoff
	setquota
	quota
	flushctx
	lsetfacl
	lgetfacl
	rsetfacl
	rgetfacl
	cp
	ls
	changelog
	changelog_clear
	fid2path
	path2fid
	data_version
	[...]
  migrate
  [...]
	For more help type: help command-name
```

First we can check how many OSTs and MDTs compose our filesystem:

```
[exact@master ~]$ lfs osts
OBDS::
0: lustre-OST0000_UUID ACTIVE
1: lustre-OST0001_UUID ACTIVE
2: lustre-OST0002_UUID ACTIVE
3: lustre-OST0003_UUID ACTIVE
```
and then

```
[exact@master ~]$ lfs  mdts
MDTS:
0: lustre-MDT0000_UUID ACTIVE
```

FS size can be checked with `df` utility and specifying `-i`  we check the number of inodes 

```
[exact@master ~]$ lfs df
UUID                   1K-blocks        Used   Available Use% Mounted on
lustre-MDT0000_UUID     27570664    12825464    14745200  47% /lustre[MDT:0]
lustre-OST0000_UUID   7807622500  5787591216  2020022376  74% /lustre[OST:0]
lustre-OST0001_UUID   7807622500  5372008856  2435586344  69% /lustre[OST:1]
lustre-OST0002_UUID   7807622500  5391058784  2416548908  69% /lustre[OST:2]
lustre-OST0003_UUID   7807622500  5468725664  2338888764  70% /lustre[OST:3]

filesystem summary:  31230490000 22019384520  9211046392  71% /lustre
```
and

```
[exact@master ~]$ lfs df -i
UUID                      Inodes       IUsed       IFree IUse% Mounted on
lustre-MDT0000_UUID     18388640    11666032     6722608  63% /lustre[MDT:0]
lustre-OST0000_UUID     15257856     2490009    12767847  16% /lustre[OST:0]
lustre-OST0001_UUID     15257856     2913607    12344249  19% /lustre[OST:1]
lustre-OST0002_UUID     15257856     2941300    12316556  19% /lustre[OST:2]
lustre-OST0003_UUID     15257856     2851728    12406128  19% /lustre[OST:3]

filesystem summary:     18388640    11666032     6722608  63% /lustre
```
We can also check the quota for each user:

```
[exact@master ~]$ lfs quota -u exact  /lustre/exact/exact/
Disk quotas for user exact (uid 669):
     Filesystem  kbytes   quota   limit   grace   files   quota   limit   grace
/lustre/exact/exact/
                119457928       0       0       -  189557       0       0       -	
```

We can now check what is the striping policy on some files/directories: 

```
[exact@master ~]$ lfs getstripe /lustre/exact/exact/MPII*
/lustre/exact/exact/MPIIO
stripe_count:   1 stripe_size:    1048576 stripe_offset:  -1
/lustre/exact/exact/MPIIO-striped
stripe_count:   4 stripe_size:    2097152 stripe_offset:  -1
[exact@master ~]$
[exact@master MPIIO-striped]$ lfs getstripe striped_over_4
striped_over_4
lmm_stripe_count:   4
lmm_stripe_size:    2097152
lmm_layout_gen:     0
lmm_stripe_offset:  0
	obdidx		 objid		 objid		 group
	     0	      34000515	    0x206ce83	             0
	     1	      40327215	    0x267582f	             0
	     3	      39804136	    0x25f5ce8	             0
	     2	      41127094	    0x2738cb6	             0

[exact@master MPIIO-striped]$
```

## Using the FS

We can search files on different OSTs and according to their striping factor by means  `lfs find` utility.
Let us find all the file striped on more than 3 OSTs
```
[exact@master exact]$ lfs find --stripe-count +3 .
./striped
./striped/yghh
./MPIIO-striped
./MPIIO-striped/striped_over_4
```

Let us see  how  files within a dir are distributed on  different OSTs:
```
[exact@master ior]$ find . | wc
    272     272    6785
[exact@master ior]$ lfs find --stripe-count 1 --ost 3 . | wc
     55      55    1430
[exact@master ior]$ lfs find --stripe-count 1 --ost 1 . | wc
     66      66    1740
[exact@master ior]$ lfs find --stripe-count 1 --ost 2 . | wc
     59      59    1463
[exact@master ior]$ lfs find --stripe-count 1 --ost 0 . | wc
     53      53    1478
```     

We can now define a new striping policy on a file and/or directory. Please note that in case of the directory all files created within the directory are actually striped accordingly to the directory parent's policy.

Let us define a new striping policy on a directory:

```
exact@master exact]$ mkdir striped2
[exact@master exact]$ lfs setstripe -c 2 -s 0 striped2
[exact@master exact]$ lfs getstripe striped2
striped2
stripe_count:   2 stripe_size:    1048576 stripe_offset:  -1
[exact@master exact]$ touch striped2/striped_over_2
[exact@master exact]$ lfs getstripe striped2/striped_over_2
striped2/striped_over_2
lmm_stripe_count:   2
lmm_stripe_size:    1048576
lmm_layout_gen:     0
lmm_stripe_offset:  2
	obdidx		 objid		 objid		 group
	     2	      41127104	    0x2738cc0	             0
	     0	      34000521	    0x206ce89	             0

[exact@master exact]$
```

We can now change such policy on the directory:

```
[exact@master exact]$ lfs setstripe -c 4 -s 0 striped2
```
and new files will be now striped on 4 OSTs:

```
[exact@master exact]$ lfs getstripe striped2/striped_over_how-many
striped2/striped_over_how-many
lmm_stripe_count:   4
lmm_stripe_size:    1048576
lmm_layout_gen:     0
lmm_stripe_offset:  3
	obdidx		 objid		 objid		 group
	     3	      39804142	    0x25f5cee	             0
	     1	      40327224	    0x2675838	             0
	     0	      34000522	    0x206ce8a	             0
	     2	      41127105	    0x2738cc1	             0
```

but we are not allowed to change the striping factor on a file: 

```
[exact@master exact]$ lfs setstripe -c 4 -s 0  striped2/striped_over_2
error on ioctl 0x4008669a for 'striped2/striped_over_2' (3): stripe already set
error: setstripe: create stripe file 'striped2/striped_over_2' failed
```





                
