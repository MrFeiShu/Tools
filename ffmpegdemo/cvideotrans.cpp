#include "cvideotrans.h"
#include <QElapsedTimer>

extern "C"
{
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
}

CVideoTrans::CVideoTrans()
    :m_pszInFile(nullptr), m_pszOutFile(nullptr), m_nIndex(-1)
{
    memset(&m_FrameInfo, 0x00, sizeof(m_FrameInfo));
}

void CVideoTrans::SetFileInfo(const QString& strInFile, const QString& strOutFile)
{
    int nInFileSize = 0;
    int nOutFileSize = 0;

    if(true == strInFile.isEmpty() || true == strOutFile.isEmpty())
    {
        qDebug("[ty] SetFileInfo invalid param.");
        return;
    }

    nInFileSize = strInFile.length() + 1;
    nOutFileSize = strOutFile.length() + 1;

    m_pszInFile = new char[nInFileSize];
    if(nullptr == m_pszInFile)
    {
        qDebug("[ty] new m_pszInFile failed.");
        return;
    }

    m_pszOutFile = new char[nOutFileSize];
    if(nullptr == m_pszOutFile)
    {
        qDebug("[ty] new m_pszOutFile failed.");
        return;
    }

    memset(m_pszInFile, 0x00, nInFileSize);
    memset(m_pszOutFile, 0x00, nOutFileSize);

    memcpy_s(m_pszInFile, nInFileSize - 1, strInFile.toLocal8Bit().data(), strInFile.length());
    memcpy_s(m_pszOutFile, nOutFileSize - 1, strOutFile.toLocal8Bit().data(), strOutFile.length());
}

void CVideoTrans::CountFrames()
{
    AVFormatContext *ifmt_ctx = NULL;
    AVPacket *pkt = NULL;
    int ret = 0;

    QElapsedTimer timer;

    timer.start();

    if (nullptr == m_pszInFile)
    {
        qDebug("[CountFrames] invalid param.");
        return;
    }

    qDebug("[CountFrames] open input file '%s'", m_pszInFile);
    if ((ret = avformat_open_input(&ifmt_ctx, m_pszInFile, 0, 0)) < 0) {
        qDebug("Could not open input file '%s'", m_pszInFile);
        return;
    }

    pkt = av_packet_alloc();
    if (!pkt) {
        qDebug("Could not allocate AVPacket\n");
        return;
    }

    while (1)
    {
        ret = av_read_frame(ifmt_ctx, pkt);
        if (ret < 0)
            break;

        int stream_index = pkt->stream_index;
        AVStream *stream = ifmt_ctx->streams[stream_index];
        enum AVMediaType type = stream->codecpar->codec_type;

        switch (type)
        {
        case AVMEDIA_TYPE_VIDEO:
        {
            m_FrameInfo.countPktVideo++;
            break;
        }
        case AVMEDIA_TYPE_AUDIO:
        {
            m_FrameInfo.countPktAudio++;
            break;
        }
        case AVMEDIA_TYPE_SUBTITLE:
        {
            m_FrameInfo.countPktSubTitle++;
            break;
        }
        case AVMEDIA_TYPE_ATTACHMENT:
        {
            m_FrameInfo.countPktAttachment++;
            break;
        }
        case AVMEDIA_TYPE_UNKNOWN:
        {
            m_FrameInfo.countPktUnkown++;
            break;
        }
        case AVMEDIA_TYPE_NB:
        {
            m_FrameInfo.countPktNb++;
            break;
        }
        case AVMEDIA_TYPE_DATA:
        {
            m_FrameInfo.countPktData++;
            break;
        }
        default:
            break;
        }
    }

    m_FrameInfo.allFrames = m_FrameInfo.countPktVideo + m_FrameInfo.countPktAudio + m_FrameInfo.countPktSubTitle + m_FrameInfo.countPktData + m_FrameInfo.countPktAttachment + m_FrameInfo.countPktUnkown + m_FrameInfo.countPktNb;

    qDebug("[ty] frameInfo.countPktVideo = %d, frameInfo.countPktAudio = %d, \
            frameInfo.countPktSubTitle = %d, frameInfo.countPktData = %d, \
            frameInfo.countPktAttachment = %d, frameInfo.countPktUnkown = %d, \
            countPktNb = %d, allFrames = %d.",
           m_FrameInfo.countPktVideo, m_FrameInfo.countPktAudio,
           m_FrameInfo.countPktSubTitle, m_FrameInfo.countPktData,
           m_FrameInfo.countPktAttachment, m_FrameInfo.countPktUnkown,
           m_FrameInfo.countPktNb, m_FrameInfo.allFrames);


    qint64 elapsed = timer.elapsed();

    av_packet_free(&pkt);
    avformat_close_input(&ifmt_ctx);
    qDebug("[CountFrames] close input file '%s'", m_pszInFile);


    qDebug("[ty] 1 elapsed: %lld", elapsed);
}

bool CVideoTrans::Translate()
{
    const AVOutputFormat *ofmt = NULL;
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVPacket *pkt = NULL;
    int ret, i;
    int countAllPkt = 0;
    int stream_index = 0;
    int *stream_mapping = NULL;
    int stream_mapping_size = 0;
    qint64 elapsed = 0;
    QElapsedTimer timer;
    int nOldPercent = 0;
    int nPercent = 0;

    timer.start();

    if(nullptr == m_pszInFile || nullptr == m_pszOutFile)
    {
        qDebug("[ty] file path is nullptr.");
        return false;
    }

    qDebug("[CVideoTrans::Translate] [index:%d] start trans video:%s to %s.", m_nIndex, m_pszInFile, m_pszOutFile);

    pkt = av_packet_alloc();
    if (!pkt) {
        qDebug("Could not allocate AVPacket\n");
        return 1;
    }

    if ((ret = avformat_open_input(&ifmt_ctx, m_pszInFile, 0, 0)) < 0) {
        qDebug("Could not open input file '%s'", m_pszInFile);
        goto end;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        qDebug("Failed to retrieve input stream information");
        goto end;
    }

    av_dump_format(ifmt_ctx, 0, m_pszInFile, 0);

    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, m_pszOutFile);
    if (!ofmt_ctx) {
        qDebug("Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }

    stream_mapping_size = ifmt_ctx->nb_streams;
    stream_mapping = (int*)av_calloc(stream_mapping_size, sizeof(*stream_mapping));
    if (!stream_mapping) {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    ofmt = ofmt_ctx->oformat;

    qDebug("[ty] ifmt_ctx->nb_streams:%d.", ifmt_ctx->nb_streams);
    for (i = 0; i < (int)ifmt_ctx->nb_streams; i++)
    {
        AVStream *out_stream;
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
        {
            stream_mapping[i] = -1;
            continue;
        }

        stream_mapping[i] = stream_index++;

        out_stream = avformat_new_stream(ofmt_ctx, NULL);
        if (!out_stream) {
            qDebug("Failed allocating output stream\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }

        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0) {
            qDebug("Failed to copy codec parameters\n");
            goto end;
        }
        out_stream->codecpar->codec_tag = 0;
    }
    //av_dump_format(ofmt_ctx, 0, m_pszOutFile, 1);

    if (!(ofmt->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&ofmt_ctx->pb, m_pszOutFile, AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            qDebug("Could not open output file '%s'", m_pszOutFile);
            goto end;
        }
    }

    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0)
    {
        qDebug("Error occurred when opening output file\n");
        goto end;
    }

    emit NotifyInfo(m_nIndex, tr("0%"));

    while (1)
    {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(ifmt_ctx, pkt);
        if (ret < 0)
        {
            qDebug("[ty] av_read_frame failed, ret=0x%08x, err:%s.", ret, av_err2str(ret));
            break;
        }

        /*
        int stream_index = pkt->stream_index;
        AVStream *stream = ifmt_ctx->streams[stream_index];
        enum AVMediaType type = stream->codecpar->codec_type;
        */

        // 发送进度到UI线程
        countAllPkt++;

        if(0 != m_FrameInfo.allFrames)
        {
            nPercent = (countAllPkt * 100) / m_FrameInfo.allFrames;

            if(nPercent > nOldPercent)
            {
                nOldPercent = nPercent;

                QString strPercent = QString::number(nPercent);
                strPercent += tr("%");

                emit NotifyInfo(m_nIndex, strPercent);
            }
        }

        in_stream  = ifmt_ctx->streams[pkt->stream_index];
        if (pkt->stream_index >= stream_mapping_size ||
            stream_mapping[pkt->stream_index] < 0)
        {
            av_packet_unref(pkt);
            continue;
        }

        pkt->stream_index = stream_mapping[pkt->stream_index];
        out_stream = ofmt_ctx->streams[pkt->stream_index];
        //log_packet(ifmt_ctx, pkt, "in");

        /* copy packet */
        av_packet_rescale_ts(pkt, in_stream->time_base, out_stream->time_base);
        pkt->pos = -1;
        //log_packet(ofmt_ctx, pkt, "out");

        ret = av_interleaved_write_frame(ofmt_ctx, pkt);
        /* pkt is now blank (av_interleaved_write_frame() takes ownership of
         * its contents and resets pkt), so that no unreferencing is necessary.
         * This would be different if one used av_write_frame(). */
        if (ret < 0)
        {
            qDebug("Error muxing packet\n");
            break;
        }
    }

    qDebug("[ty] countAllPkt = %d.", countAllPkt);
    emit NotifyInfo(m_nIndex, tr("100%"));

    av_write_trailer(ofmt_ctx);
end:
    av_packet_free(&pkt);

    avformat_close_input(&ifmt_ctx);

    elapsed = timer.elapsed();
    qDebug("[ty] 2 elapsed:%lld", elapsed);

    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

    av_freep(&stream_mapping);

    if (ret < 0 && ret != AVERROR_EOF) {
        //fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        qDebug("Error occurred: %s\n", av_err2str(ret));
        return 1;
    }

    return 0;
}


void CVideoTrans::run()
{
    qDebug("[ty]CVideoTrans::run enter.");

    if(nullptr == m_pSem)
    {
        qDebug("[CVideoTrans::run] m_pSem is nullptr.");
        return;
    }

    m_pSem->acquire();

    CountFrames();

    Translate();

    m_pSem->release();
}

void CVideoTrans::InitTrans(const QSemaphore* pSem, const int nIndex, const QString& strInFile, const QString& strOutFile)
{
    m_nIndex = nIndex;
    m_pSem = const_cast<QSemaphore*>(pSem);

    SetFileInfo(strInFile, strOutFile);
}
