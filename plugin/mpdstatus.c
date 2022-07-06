//  gcc -o mpdstatus mpdstatus.c -lmpdclient
#include <mpd/client.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *, const char *, int, char *, int );

/* smprintf may be unsafe, or use 'asprintf' */
char *smprintf(char *fmt, ...) {
  va_list fmtargs;
  char *ret;
  int len;

  va_start(fmtargs, fmt);
  /* first vsnprintf: detection of str */
  len = vsnprintf(NULL, 0, fmt, fmtargs);
  va_end(fmtargs);

  ret = malloc(++len);
  if (ret == NULL) {
    perror("malloc");
    exit(1);
  }

  va_start(fmtargs, fmt);
  /* second vsnprintf: assign str to *ret actually */
  vsnprintf(ret, len, fmt, fmtargs);
  va_end(fmtargs);

  return ret;
}

/* simple function to retrieve mpd status */
char *getmpdstat() {
  struct mpd_song *song = NULL;
  char *mpdstat = NULL, *pos_pl = NULL, *play_flag = NULL;
  char *retstr = NULL;
  int elapsed = 0, total = 0, remain = 0, br = 0;
  struct mpd_connection *conn;
  if (!(conn = mpd_connection_new("localhost", 6600, 30000)) ||
      mpd_connection_get_error(conn)) {
    return smprintf(" Not connected! \n");
  }

  mpd_command_list_begin(conn, true);
  mpd_send_status(conn);
  mpd_send_current_song(conn);
  mpd_command_list_end(conn);

  struct mpd_status *theStatus = mpd_recv_status(conn);
  if ((theStatus) && (mpd_status_get_state(theStatus) == MPD_STATE_PLAY ||
                      mpd_status_get_state(theStatus) == MPD_STATE_PAUSE)) {
    mpd_response_next(conn);
    song = mpd_recv_song(conn);

    int len = strlen(mpd_song_get_tag(song, MPD_TAG_TITLE, 0));
    char title[31] = {""};
    trim(title, mpd_song_get_tag(song, MPD_TAG_TITLE, 0), 30, "...", 3);

    char artist[16];
    trim(artist, mpd_song_get_tag(song, MPD_TAG_ARTIST, 0), 15, "...", 3);

    char date[5] = {""};
    strncpy(date, mpd_song_get_tag(song, MPD_TAG_DATE, 0) ?: "none", 4);
    date[4] = '\0';

    pos_pl = smprintf("%5d/%d", mpd_status_get_song_pos(theStatus) + 1,
                      mpd_status_get_queue_length(theStatus));
    play_flag = smprintf("%s%s%s", mpd_status_get_random(theStatus) ? "z" : "-",
                         mpd_status_get_repeat(theStatus) ? "r" : "-",
                         mpd_status_get_single(theStatus) ? "s" : "-");
    mpdstat = mpd_status_get_state(theStatus) == MPD_STATE_PLAY ? "P" : "Z";

    //elapsed = mpd_status_get_elapsed_time(theStatus);
    //total = mpd_status_get_total_time(theStatus);
    //remain = total - elapsed;
    br = mpd_status_get_kbit_rate(theStatus);
    mpd_song_free(song);
    retstr =
        smprintf(" %s - %s  [%s%s %s] %dk @%s \n", title, artist,
                 mpdstat, play_flag, pos_pl, br, date);
  } else
    retstr = smprintf(" [S] \n");
  mpd_response_finish(conn);
  mpd_connection_free(conn);
  return retstr;
}

void trim(char *trim, const char *raw, int maxlen, char *altstr, int altstrlen)
{
    int rawlen;
    rawlen = snprintf(trim, maxlen+1, "%s", raw);
    /* printf("%d\n", rawlen); */
    if ( rawlen > maxlen)
        snprintf(trim+maxlen-altstrlen, altstrlen+1, "%s", altstr); // -1 for end '\0'
}

//int main(int argc, char *argv[])
//{
//    printf("%s", getmpdstat());
//    return 0;
//}
