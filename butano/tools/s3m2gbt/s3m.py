# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

from s3m2gbt import kaitaistruct
from s3m2gbt.kaitaistruct import KaitaiStruct, KaitaiStream, BytesIO
from enum import Enum


class S3m(KaitaiStruct):
    """Scream Tracker 3 module is a tracker music file format that, as all
    tracker music, bundles both sound samples and instructions on which
    notes to play. It originates from a Scream Tracker 3 music editor
    (1994) by Future Crew, derived from original Scream Tracker 2 (.stm)
    module format.
    
    Instrument descriptions in S3M format allow to use either digital
    samples or setup and control AdLib (OPL2) synth.
    
    Music is organized in so called `patterns`. "Pattern" is a generally
    a 64-row long table, which instructs which notes to play on which
    time measure. "Patterns" are played one-by-one in a sequence
    determined by `orders`, which is essentially an array of pattern IDs
    - this way it's possible to reuse certain patterns more than once
    for repetitive musical phrases.
    
    .. seealso::
       Source - http://hackipedia.org/browse.cgi/File%20formats/Music%20tracker/S3M%2c%20ScreamTracker%203/Scream%20Tracker%203.20%20by%20Future%20Crew.txt
    """
    def __init__(self, _io, _parent=None, _root=None):
        self._io = _io
        self._parent = _parent
        self._root = _root if _root else self
        self._read()

    def _read(self):
        self.song_name = KaitaiStream.bytes_terminate(self._io.read_bytes(28), 0, False)
        self.magic1 = self._io.read_bytes(1)
        if not self.magic1 == b"\x1A":
            raise kaitaistruct.ValidationNotEqualError(b"\x1A", self.magic1, self._io, u"/seq/1")
        self.file_type = self._io.read_u1()
        self.reserved1 = self._io.read_bytes(2)
        self.num_orders = self._io.read_u2le()
        self.num_instruments = self._io.read_u2le()
        self.num_patterns = self._io.read_u2le()
        self.flags = self._io.read_u2le()
        self.version = self._io.read_u2le()
        self.samples_format = self._io.read_u2le()
        self.magic2 = self._io.read_bytes(4)
        if not self.magic2 == b"\x53\x43\x52\x4D":
            raise kaitaistruct.ValidationNotEqualError(b"\x53\x43\x52\x4D", self.magic2, self._io, u"/seq/10")
        self.global_volume = self._io.read_u1()
        self.initial_speed = self._io.read_u1()
        self.initial_tempo = self._io.read_u1()
        self.is_stereo = self._io.read_bits_int_be(1) != 0
        self.master_volume = self._io.read_bits_int_be(7)
        self._io.align_to_byte()
        self.ultra_click_removal = self._io.read_u1()
        self.has_custom_pan = self._io.read_u1()
        self.reserved2 = self._io.read_bytes(8)
        self.ofs_special = self._io.read_u2le()
        self.channels = [None] * (32)
        for i in range(32):
            self.channels[i] = S3m.Channel(self._io, self, self._root)

        self.orders = self._io.read_bytes(self.num_orders)
        self.instruments = [None] * (self.num_instruments)
        for i in range(self.num_instruments):
            self.instruments[i] = S3m.InstrumentPtr(self._io, self, self._root)

        self.patterns = [None] * (self.num_patterns)
        for i in range(self.num_patterns):
            self.patterns[i] = S3m.PatternPtr(self._io, self, self._root)

        if self.has_custom_pan == 252:
            self.channel_pans = [None] * (32)
            for i in range(32):
                self.channel_pans[i] = S3m.ChannelPan(self._io, self, self._root)



    class ChannelPan(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.reserved1 = self._io.read_bits_int_be(2)
            self.has_custom_pan = self._io.read_bits_int_be(1) != 0
            self.reserved2 = self._io.read_bits_int_be(1) != 0
            self.pan = self._io.read_bits_int_be(4)


    class PatternCell(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.has_fx = self._io.read_bits_int_be(1) != 0
            self.has_volume = self._io.read_bits_int_be(1) != 0
            self.has_note_and_instrument = self._io.read_bits_int_be(1) != 0
            self.channel_num = self._io.read_bits_int_be(5)
            self._io.align_to_byte()
            if self.has_note_and_instrument:
                self.note = self._io.read_u1()

            if self.has_note_and_instrument:
                self.instrument = self._io.read_u1()

            if self.has_volume:
                self.volume = self._io.read_u1()

            if self.has_fx:
                self.fx_type = self._io.read_u1()

            if self.has_fx:
                self.fx_value = self._io.read_u1()



    class PatternCells(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.cells = []
            i = 0
            while not self._io.is_eof():
                self.cells.append(S3m.PatternCell(self._io, self, self._root))
                i += 1



    class Channel(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.is_disabled = self._io.read_bits_int_be(1) != 0
            self.ch_type = self._io.read_bits_int_be(7)


    class SwappedU3(KaitaiStruct):
        """Custom 3-byte integer, stored in mixed endian manner."""
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.hi = self._io.read_u1()
            self.lo = self._io.read_u2le()

        @property
        def value(self):
            if hasattr(self, '_m_value'):
                return self._m_value if hasattr(self, '_m_value') else None

            self._m_value = (self.lo | (self.hi << 16))
            return self._m_value if hasattr(self, '_m_value') else None


    class Pattern(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.size = self._io.read_u2le()
            self._raw_body = self._io.read_bytes((self.size - 2))
            _io__raw_body = KaitaiStream(BytesIO(self._raw_body))
            self.body = S3m.PatternCells(_io__raw_body, self, self._root)


    class PatternPtr(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.paraptr = self._io.read_u2le()

        @property
        def body(self):
            if hasattr(self, '_m_body'):
                return self._m_body if hasattr(self, '_m_body') else None

            _pos = self._io.pos()
            self._io.seek((self.paraptr * 16))
            self._m_body = S3m.Pattern(self._io, self, self._root)
            self._io.seek(_pos)
            return self._m_body if hasattr(self, '_m_body') else None


    class InstrumentPtr(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.paraptr = self._io.read_u2le()

        @property
        def body(self):
            if hasattr(self, '_m_body'):
                return self._m_body if hasattr(self, '_m_body') else None

            _pos = self._io.pos()
            self._io.seek((self.paraptr * 16))
            self._m_body = S3m.Instrument(self._io, self, self._root)
            self._io.seek(_pos)
            return self._m_body if hasattr(self, '_m_body') else None


    class Instrument(KaitaiStruct):

        class InstTypes(Enum):
            sample = 1
            melodic = 2
            bass_drum = 3
            snare_drum = 4
            tom = 5
            cymbal = 6
            hihat = 7
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.type = KaitaiStream.resolve_enum(S3m.Instrument.InstTypes, self._io.read_u1())
            self.filename = KaitaiStream.bytes_terminate(self._io.read_bytes(12), 0, False)
            _on = self.type
            if _on == S3m.Instrument.InstTypes.sample:
                self.body = S3m.Instrument.Sampled(self._io, self, self._root)
            else:
                self.body = S3m.Instrument.Adlib(self._io, self, self._root)
            self.tuning_hz = self._io.read_u4le()
            self.reserved2 = self._io.read_bytes(12)
            self.sample_name = KaitaiStream.bytes_terminate(self._io.read_bytes(28), 0, False)
            self.magic = self._io.read_bytes(4)
            if not self.magic == b"\x53\x43\x52\x53":
                raise kaitaistruct.ValidationNotEqualError(b"\x53\x43\x52\x53", self.magic, self._io, u"/types/instrument/seq/6")

        class Sampled(KaitaiStruct):
            def __init__(self, _io, _parent=None, _root=None):
                self._io = _io
                self._parent = _parent
                self._root = _root if _root else self
                self._read()

            def _read(self):
                self.paraptr_sample = S3m.SwappedU3(self._io, self, self._root)
                self.len_sample = self._io.read_u4le()
                self.loop_begin = self._io.read_u4le()
                self.loop_end = self._io.read_u4le()
                self.default_volume = self._io.read_u1()
                self.reserved1 = self._io.read_u1()
                self.is_packed = self._io.read_u1()
                self.flags = self._io.read_u1()

            @property
            def sample(self):
                if hasattr(self, '_m_sample'):
                    return self._m_sample if hasattr(self, '_m_sample') else None

                _pos = self._io.pos()
                self._io.seek((self.paraptr_sample.value * 16))
                self._m_sample = self._io.read_bytes(self.len_sample)
                self._io.seek(_pos)
                return self._m_sample if hasattr(self, '_m_sample') else None


        class Adlib(KaitaiStruct):
            def __init__(self, _io, _parent=None, _root=None):
                self._io = _io
                self._parent = _parent
                self._root = _root if _root else self
                self._read()

            def _read(self):
                self.reserved1 = self._io.read_bytes(3)
                if not self.reserved1 == b"\x00\x00\x00":
                    raise kaitaistruct.ValidationNotEqualError(b"\x00\x00\x00", self.reserved1, self._io, u"/types/instrument/types/adlib/seq/0")
                self._unnamed1 = self._io.read_bytes(16)




