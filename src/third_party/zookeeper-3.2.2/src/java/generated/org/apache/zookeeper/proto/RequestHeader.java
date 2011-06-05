// File generated by hadoop record compiler. Do not edit.
package org.apache.zookeeper.proto;

import java.util.*;
import org.apache.jute.*;
public class RequestHeader implements Record {
  private int xid;
  private int type;
  public RequestHeader() {
  }
  public RequestHeader(
        int xid,
        int type) {
    this.xid=xid;
    this.type=type;
  }
  public int getXid() {
    return xid;
  }
  public void setXid(int m_) {
    xid=m_;
  }
  public int getType() {
    return type;
  }
  public void setType(int m_) {
    type=m_;
  }
  public void serialize(OutputArchive a_, String tag) throws java.io.IOException {
    a_.startRecord(this,tag);
    a_.writeInt(xid,"xid");
    a_.writeInt(type,"type");
    a_.endRecord(this,tag);
  }
  public void deserialize(InputArchive a_, String tag) throws java.io.IOException {
    a_.startRecord(tag);
    xid=a_.readInt("xid");
    type=a_.readInt("type");
    a_.endRecord(tag);
}
  public String toString() {
    try {
      java.io.ByteArrayOutputStream s =
        new java.io.ByteArrayOutputStream();
      CsvOutputArchive a_ = 
        new CsvOutputArchive(s);
      a_.startRecord(this,"");
    a_.writeInt(xid,"xid");
    a_.writeInt(type,"type");
      a_.endRecord(this,"");
      return new String(s.toByteArray(), "UTF-8");
    } catch (Throwable ex) {
      ex.printStackTrace();
    }
    return "ERROR";
  }
  public void write(java.io.DataOutput out) throws java.io.IOException {
    BinaryOutputArchive archive = new BinaryOutputArchive(out);
    serialize(archive, "");
  }
  public void readFields(java.io.DataInput in) throws java.io.IOException {
    BinaryInputArchive archive = new BinaryInputArchive(in);
    deserialize(archive, "");
  }
  public int compareTo (Object peer_) throws ClassCastException {
    if (!(peer_ instanceof RequestHeader)) {
      throw new ClassCastException("Comparing different types of records.");
    }
    RequestHeader peer = (RequestHeader) peer_;
    int ret = 0;
    ret = (xid == peer.xid)? 0 :((xid<peer.xid)?-1:1);
    if (ret != 0) return ret;
    ret = (type == peer.type)? 0 :((type<peer.type)?-1:1);
    if (ret != 0) return ret;
     return ret;
  }
  public boolean equals(Object peer_) {
    if (!(peer_ instanceof RequestHeader)) {
      return false;
    }
    if (peer_ == this) {
      return true;
    }
    RequestHeader peer = (RequestHeader) peer_;
    boolean ret = false;
    ret = (xid==peer.xid);
    if (!ret) return ret;
    ret = (type==peer.type);
    if (!ret) return ret;
     return ret;
  }
  public int hashCode() {
    int result = 17;
    int ret;
    ret = (int)xid;
    result = 37*result + ret;
    ret = (int)type;
    result = 37*result + ret;
    return result;
  }
  public static String signature() {
    return "LRequestHeader(ii)";
  }
}