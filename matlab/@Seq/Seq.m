classdef Seq < handle
  properties(Hidden)
    filename='';
    nframes=0;
  end
  methods(Static,Access='public')
    function self=Seq(filename)
      self.filename=filename;
      self.nframes=SeqLength(filename);
    end
  end
  methods
    function im=read(h,idx)
      im=SeqRead(h.filename,idx);
    end
    function n=length(h,idx)
      n=h.nframes;
    end
  end
end