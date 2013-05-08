function im=SeqRead(filename,idx)
  im=mexSeqRead(char(filename),uint32(idx));
end