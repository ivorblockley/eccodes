! Copyright 2005-2015 ECMWF.
!
! This software is licensed under the terms of the Apache Licence Version 2.0
! which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
! 
! In applying this licence, ECMWF does not waive the privileges and immunities granted to it by
! virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
!
!
!
!  Description: how to create a new BUFR message by cloning 
!               an existing message.
!
!
!
program bufr_clone
  use eccodes
  implicit none
  integer                                       :: err,i,iret
  integer                                       :: nx, ny
  integer                                       :: infile,outfile
  integer                                       :: ibufr_in
  integer                                       :: ibufr_out
  character(len=2)                              :: step
  double precision, dimension(:,:), allocatable :: field2D
  
  !open source file 
  call codes_open_file(infile,'../../data/bufr/syno_multi.bufr','r')
  
  ! open target file
  call codes_open_file(outfile,'out.clone.f.bufr','w')
  
  ! the first bufr message is loaded from file
  ! ibufr is the bufr id to be used in subsequent calls
  call codes_bufr_new_from_file(infile,ibufr_in,iret)

  do while (iret/=CODES_END_OF_FILE)
    
    ! clone the current handle
    call codes_clone(ibufr_in, ibufr_out)

    ! This is the place where you may wish to modify the clone 
    ! E.g. we change the centre 
    call codes_set(ibufr_out,'centre',250)
 
    ! write cloned messages to a file
    call codes_write(ibufr_out,outfile)
    
    ! relase the clone's handle
    call codes_release(ibufr_out)
  
    ! relase the sources 's handle
    call codes_release(ibufr_in)
    
    ! next message from source
    call codes_bufr_new_from_file(infile,ibufr_in,iret)
    
  end do
    
  call codes_close_file(infile)
  call codes_close_file(outfile)

end program bufr_clone
