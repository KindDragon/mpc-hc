/*
 * (C) 2006-2012 see Authors.txt
 *
 * This file is part of MPC-HC.
 *
 * MPC-HC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-HC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

typedef enum {
    NALU_TYPE_SLICE    = 1,
    NALU_TYPE_DPA      = 2,
    NALU_TYPE_DPB      = 3,
    NALU_TYPE_DPC      = 4,
    NALU_TYPE_IDR      = 5,
    NALU_TYPE_SEI      = 6,
    NALU_TYPE_SPS      = 7,
    NALU_TYPE_PPS      = 8,
    NALU_TYPE_AUD      = 9,
    NALU_TYPE_EOSEQ    = 10,
    NALU_TYPE_EOSTREAM = 11,
    NALU_TYPE_FILL     = 12
} NALU_TYPE;

class CH264Nalu
{
private :
    int forbidden_bit;       //! should be always FALSE
    int nal_reference_idc;   //! NALU_PRIORITY_xxxx
    NALU_TYPE nal_unit_type; //! NALU_TYPE_xxxx

    int m_nNALStartPos;      //! NALU start (including startcode / size)
    int m_nNALDataPos;       //! Useful part

    BYTE* m_pBuffer;
    int m_nCurPos;
    int m_nNextRTP;
    int m_nSize;
    int m_nNALSize;

    bool MoveToNextAnnexBStartcode();
    bool MoveToNextRTPStartcode();

public :
    CH264Nalu() { SetBuffer(NULL, 0, 0); }

    NALU_TYPE GetType() const { return nal_unit_type; };
    bool IsRefFrame() const { return (nal_reference_idc != 0); };

    int GetDataLength() const { return m_nCurPos - m_nNALDataPos; };
    BYTE* GetDataBuffer() { return m_pBuffer + m_nNALDataPos; };
    int GetRoundedDataLength() const {
        int nSize = m_nCurPos - m_nNALDataPos;
        return nSize + 128 - (nSize % 128);
    }

    int GetLength() const { return m_nCurPos - m_nNALStartPos; };
    BYTE* GetNALBuffer() { return m_pBuffer + m_nNALStartPos; };
    bool IsEOF() const { return m_nCurPos >= m_nSize; };

    void SetBuffer(BYTE* pBuffer, int nSize, int nNALSize);
    bool ReadNext();
};
